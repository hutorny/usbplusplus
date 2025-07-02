/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ft/usbsys.cpp - USB system for functional test
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */
#include "libusbi.hpp"
#include "usbsys.hpp"
#include <usbplusplus/dispatch.hpp>
#include <devices.hpp>
#include <utf8.hpp>

#include <array>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <source_location>
#include <functional>
#include <vector>
#include <map>
#include <tuple>
#include <thread>

#pragma GCC diagnostic ignored "-Wold-style-cast" // casts from libusb macros

namespace usbplusplus {
namespace ft {
namespace {
using device_descriptor_data = std::array<uint8_t, 18>;
using device_qualifier_data = std::array<uint8_t, 10>;
using descriptor_data = std::vector<uint8_t>;
using descriptor_vect = std::vector<descriptor_data>;
constexpr unsigned config_value_offset = 5;
constexpr unsigned vendor_offset = 5;
static_assert(config_value_offset == offsetof(usb1::Configuration<Empty>, bConfigurationValue));

struct device_item {
    device_info info;
    device_descriptor_data device_descriptor;
    device_qualifier_data device_qualifier;
    descriptor_vect configurations_descriptors;
    string_getter strings;
    std::source_location location;
    uint8_t active_config_index {};
    std::vector<AlternateSetting> alternate_settings {};
};

auto& device_list() { // COFU
    static std::map<devaddr, device_item> list{};
    return list;
}

auto make_device_descriptor_data(descriptor src) {
    device_descriptor_data result {};
    std::copy(src.begin(), src.end(), result.begin());
    return result;
}

auto make_device_quaifier_data(descriptor src) {
    device_qualifier_data result {};
    std::copy(src.begin(), src.end(), result.begin());
    return result;
}

auto make_descriptor_data(descriptor src) {
    descriptor_data result {};
    result.insert(result.begin(), src.begin(), src.end());
    return result;
}

auto make_descriptor_vect(descriptor_list src) {
    descriptor_vect result {};
    for(auto s : src)
        result.push_back(make_descriptor_data(s));
    return result;
}

void copy_device_info(libusb_device& d, const device_info& info) {
    d.bus_number = info.bus_number.get();
    d.device_address = static_cast<uint8_t>(info.device_address);
    d.port_number = info.port_number.get();
    d.speed = static_cast<libusb_speed>(info.speed);
}

int copy_descriptor_data(const auto& data, void *buffer, size_t len) {
    len = std::min(data.size(), len);
    std::memcpy(buffer, data.data(), len);
    return static_cast<int>(len);
}

void copy_device_data(libusb_device& d, const device_item& item) {
    copy_device_info(d, item.info);
    copy_descriptor_data(item.device_descriptor, &d.device_descriptor, sizeof(d.device_descriptor));
}

unsigned long make_session_id(device_info info) {
    return static_cast<unsigned long>(info.bus_number.get() << 8 | static_cast<uint8_t>(info.device_address));
}

}

void usbsys::add(device_info info, std::source_location loc, descriptor devdescr, descriptor qualifier, descriptor_list configs,
        string_getter strgetter) {
    const auto found = device_list().find(info.device_address);
    if( found != device_list().end() )
        throw std::logic_error{ "USB device address " + std::to_string(static_cast<unsigned>(info.device_address)) +
                "requested at " + loc +
                " already in use at " + found->second.location
    };
    if( devdescr.size() != std::tuple_size<device_descriptor_data>{}) {
        throw std::logic_error{ "Device descriptor has wrong size " + std::to_string(devdescr.size()) + " at " + loc };
    }
    if( qualifier.size() != 0 && qualifier.size() != std::tuple_size<device_qualifier_data>{}) {
        throw std::logic_error{ "Device qualifier has wrong size " + std::to_string(qualifier.size()) + " at " + loc };
    }
    device_list().emplace(info.device_address, device_item{
        info,
        make_device_descriptor_data(devdescr),
        make_device_quaifier_data(qualifier),
        make_descriptor_vect(configs),
        strgetter,
        loc,
        0U
    });
}

void usbsys::remove(devaddr addr) {
    device_list().erase(device_list().find(addr));
}

static int get_device_list(struct libusb_context *ctx, struct discovered_devs **discdevs) {
    for(const auto& d : device_list()) {
        libusb_device *dev = usbi_alloc_device(ctx, make_session_id(d.second.info));
        if (dev == NULL)
            return LIBUSB_ERROR_NO_MEM;
        copy_device_data(*dev, d.second);
        discovered_devs_append(*discdevs, dev);
    }
    return LIBUSB_SUCCESS;
}

static int get_config_descriptor(libusb_device *dev, uint8_t config_index, void *buffer, size_t len) {
    auto found = device_list().find(devaddr(dev->device_address));
    if (found == device_list().end())
        return LIBUSB_ERROR_NOT_FOUND;
    if (config_index >= found->second.configurations_descriptors.size())
        return LIBUSB_ERROR_INVALID_PARAM;
    return copy_descriptor_data(found->second.configurations_descriptors[config_index], buffer, len);
}

static int get_active_config_descriptor(libusb_device *dev, void *buffer, size_t len) {
    auto found = device_list().find(devaddr(dev->device_address));
    if (found == device_list().end())
        return LIBUSB_ERROR_NOT_FOUND;
    if (found->second.active_config_index >= found->second.configurations_descriptors.size())
        return LIBUSB_ERROR_NOT_FOUND;
    return get_config_descriptor(dev, found->second.active_config_index, buffer, len);
}

using find_config_result = std::tuple<const descriptor_data*, std::size_t, device_item*>;

static find_config_result find_config_by_value(libusb_device *dev, uint8_t value) {
    auto found = device_list().find(devaddr(dev->device_address));
    if (found == device_list().end())
        return { };
    auto& configs { found->second.configurations_descriptors };
    auto config = std::find_if(configs.begin(), configs.end(), [value](const auto& data) {
        return data[config_value_offset] == value;
    });
    if (config == configs.end())
        return { };
    return {&*config, static_cast<std::size_t>(config - configs.begin()), &found->second };
}

static int get_config_descriptor_by_value(libusb_device *dev, uint8_t value, void **buffer) {
    auto [config, index, devitem] = find_config_by_value(dev, value);
    if (config == nullptr)
        return LIBUSB_ERROR_NOT_FOUND;
    *buffer = const_cast<void*>(static_cast<const void*>(config->data()));
    return static_cast<int>(config->size());
}

static int get_configuration(libusb_device_handle *dev_handle, uint8_t *config) {
    auto found = device_list().find(devaddr(dev_handle->dev->device_address));
    if (found == device_list().end())
        return LIBUSB_ERROR_NOT_FOUND;
    if (found->second.active_config_index >= found->second.configurations_descriptors.size())
        return LIBUSB_ERROR_NOT_FOUND;
    *config = found->second.configurations_descriptors[found->second.active_config_index][config_value_offset];
    return LIBUSB_SUCCESS;
}

static int set_configuration(libusb_device_handle *dev_handle, int config_value) {
    auto [config, index, devitem] = find_config_by_value(dev_handle->dev, static_cast<uint8_t>(config_value));
    if (config == nullptr)
        return LIBUSB_ERROR_NOT_FOUND;
    devitem->active_config_index = static_cast<uint8_t>(index);
    return LIBUSB_SUCCESS;
}

static auto get_device_descriptor(ControlPacket packet, device_item& dev, response resp) {
    resp.send(packet.wLength.get(), dev.device_descriptor);
    return true;
}

static auto get_device_qualifier(ControlPacket packet, device_item& dev, response resp) {
    if (dev.device_qualifier[0] != 0) {
        resp.send(packet.wLength.get(), dev.device_qualifier);
    }
    return true;
}

static auto get_configuration_descriptor(ControlPacket packet, device_item& dev, response resp) {
    if (packet.descriptor_index() <= dev.configurations_descriptors.size()) {
        resp.send(packet.wLength.get(), dev.configurations_descriptors[packet.descriptor_index()]);
    }
    return true;
}

static auto get_string(ControlPacket packet, device_item& dev, response resp) {
    auto str = dev.strings(packet.descriptor_index(), packet.language_id());
    if (str == nullptr) return true;
    auto len = packet.wLength.get();
    if (packet.wLength.get() == 0) {
        len = str[0];
    }
    resp.send(len, str);
    return true;
}

static auto get_interface_descriptor(ControlPacket, device_item&, response) {
    // not used in functional tests
    return true;
}

static auto get_config(ControlPacket, device_item& dev, response resp) {
    if (dev.active_config_index <= dev.configurations_descriptors.size())
        resp.send(dev.configurations_descriptors[dev.active_config_index][config_value_offset]);
    return true;
}

static auto get_status(ControlPacket, device_item&, response resp) {
    resp.send(DeviceStatus_t::SelfPowered);
    return true;
}

static auto get_interface(ControlPacket packet, device_item& dev, response resp) {
    if(packet.interface_index().get() < dev.alternate_settings.size())
        resp.send(dev.alternate_settings[packet.interface_index().get()]);
    return true;
}

static auto set_interface(ControlPacket packet, device_item& dev, response) {
    dev.alternate_settings[packet.interface_index().get()] = packet.alternate_setting();
    return true;
}

static auto get_interface_association(ControlPacket, device_item&, response) {
    // not used in functional tests
    return true;
}

static auto get_debug_descriptor(ControlPacket packet, device_item&, response resp) {
    static constexpr std::array<uint8_t, 4> debug { 0, 0, 0, 0 };
    resp.send(packet.wLength.get(), debug);
    return true;
}

// Dispatches standard requests to the functions above
using request_dispatcher = dispatch::dispatcher<
    dispatch::to<get_device_descriptor, dispatch::when<DescriptorType_t::DEVICE>{}>,
    dispatch::to<get_device_qualifier, dispatch::when<DescriptorType_t::DEVICE_QUALIFIER>{}>,
    dispatch::to<get_configuration_descriptor, dispatch::when<DescriptorType_t::CONFIGURATION>{}>,
    dispatch::to<get_interface_descriptor, dispatch::when<DescriptorType_t::INTERFACE>{}>,
    dispatch::to<get_interface_association, dispatch::when<DescriptorType_t::INTERFACE_ASSOCIATION>{}>,
    dispatch::to<get_debug_descriptor, dispatch::when<DescriptorType_t::DEBUG>{}>,
    dispatch::to<get_string, dispatch::when<DescriptorType_t::STRING>{}>,
    dispatch::to<get_config, dispatch::when<RequestCode_t::GET_CONFIGURATION>{}>,
    dispatch::to<get_status, dispatch::when<RequestCode_t::GET_STATUS, Recipient_t::Device>{}>,
    dispatch::to<get_interface, dispatch::when<RequestCode_t::GET_INTERFACE>{}>,
    dispatch::to<set_interface, dispatch::when<RequestCode_t::SET_INTERFACE>{}>
>;

static int submit_transfer(usbi_transfer *itransfer) {
    if (itransfer->dev == nullptr) {
        return LIBUSB_ERROR_INVALID_PARAM;
    }
    libusb_transfer& transfer = *USBI_TRANSFER_TO_LIBUSB_TRANSFER(itransfer);
    if (transfer.type != LIBUSB_TRANSFER_TYPE_CONTROL) {
        return LIBUSB_ERROR_NOT_SUPPORTED;
    }

    auto found = device_list().find(devaddr(itransfer->dev->device_address));
    if (found == device_list().end()) {
        return LIBUSB_ERROR_NOT_FOUND;
    }

    const ControlPacket& packet = *reinterpret_cast<ControlPacket*>(transfer.buffer);
    int error = LIBUSB_SUCCESS;
    if (!request_dispatcher{}(packet, found->second, response{libusb_control_transfer_get_data(&transfer), itransfer->transferred, error})) {
        error = LIBUSB_ERROR_NOT_SUPPORTED;
    }
    std::thread complete_transfer_later{[](usbi_transfer *itrans, libusb_transfer* trans) {
        using namespace std::literals;
        std::this_thread::sleep_for(1ms);
        *static_cast<int*>(trans->user_data) = 1;
        usbi_signal_event(&itrans->dev->ctx->event);
        usbi_handle_transfer_completion(itrans, LIBUSB_TRANSFER_COMPLETED);
    }, itransfer, &transfer};
    complete_transfer_later.detach();
    transfer.actual_length = itransfer->transferred;
    return error;
}

static int open_device(libusb_device_handle* ludh){
    return device_list().contains(devaddr(ludh->dev->device_address)) ? LIBUSB_SUCCESS : LIBUSB_ERROR_NO_DEVICE;
}

} // namespace ft
} // namespace usbplusplus


#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

using namespace usbplusplus::ft;
// Substitutes libusb backend when libusb is linked statically
const struct usbi_os_backend usbi_backend = {
    .name = "USB++ Functional Tests Backend",
    .caps = 0,
    .get_device_list = get_device_list,
    .open = open_device,
    .close = [](libusb_device_handle*){},
    .get_active_config_descriptor = get_active_config_descriptor,
    .get_config_descriptor = get_config_descriptor,
    .get_config_descriptor_by_value = get_config_descriptor_by_value,
    .get_configuration = get_configuration,
    .set_configuration = set_configuration,
    .claim_interface = [](libusb_device_handle*, uint8_t)->int { return LIBUSB_ERROR_NOT_SUPPORTED; },
    .release_interface = [](libusb_device_handle*, uint8_t)->int { return LIBUSB_ERROR_NOT_SUPPORTED; },
    .set_interface_altsetting = [](libusb_device_handle*, uint8_t,  uint8_t)->int { return LIBUSB_ERROR_NOT_SUPPORTED; },
    .clear_halt = [](libusb_device_handle*, unsigned char)->int { return LIBUSB_ERROR_NOT_SUPPORTED; },
    .submit_transfer = submit_transfer,
    .cancel_transfer = [](usbi_transfer*)->int { return LIBUSB_SUCCESS; },
};
