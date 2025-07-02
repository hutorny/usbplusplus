/* Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ft/usbsys.hpp - USB system for testing purpose
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <span>
#include <stdexcept>
#include <source_location>

#include <usbplusplus/usbplusplus.hpp>
#include "addresses.hpp"

extern "C" {
struct libusb_device_handle;
struct libusb_device;
struct libusb_context;
struct discovered_devs;
struct usbi_transfer;
}

namespace usbplusplus {
namespace ft {
using descriptor = std::span<const std::uint8_t>;
using descriptor_list = std::initializer_list<descriptor>;
using string_getter = const uint8_t* (*)(Index::type index, LanguageIdentifier);

enum class libusbspeed : uint8_t {
    unknown, low, full, high, super, super_plus, super_plus_x2
};

struct device_info {
    devaddr device_address;
    Index bus_number;
    Index port_number;
    libusbspeed speed;
};

inline constexpr bool operator==(BCD l, BCD r) noexcept {
    return l.get() == r.get();
}

inline std::string operator+(const std::string& lhs, std::source_location location) {
    std::string str{};
    str.reserve(str.size() + std::strlen(location.file_name()) + sizeof("9999"));
    str = lhs;
    str += location.file_name();
    str += ':';
    str += std::to_string(location.line());
    return str;
}

// Implements USB "bus"
class usbsys final {
public:
private:
    static constexpr uint8_t first_test_bus_id = 240; // to avoid collision with real USB bus
    static void add(device_info, std::source_location loc, descriptor, descriptor, descriptor_list, string_getter);
    static void remove(devaddr);
    static constexpr device_info make_device_info(devaddr device_address, BCD bcdusb) {
        return {
            device_address,
            Index{static_cast<uint8_t>(first_test_bus_id + (static_cast<uint8_t>(device_address) >> 4))},
            Index{static_cast<uint8_t>(static_cast<uint8_t>(device_address) & 0xF)},
            bcdusb == 2.00_bcd ? libusbspeed::high : libusbspeed::full
        };
    }

    template<typename Strings, typename ... Configurations>
    requires requires(Index::type index, LanguageIdentifier lang, const Configurations& ... config) {
        Strings::get(index, lang);
        (config.ptr(),...);
        (Configurations::length(),...);
        (Configurations::totallength(),...);
    }
    friend class usbdevice;
};

struct address_with_location {
    address_with_location(devaddr addr, std::source_location loc= std::source_location::current())
      : address{addr}, location{loc} {}
    devaddr address;
    std::source_location location;
};

template<typename Strings, typename ... Configurations>
requires requires(Index::type index, LanguageIdentifier lang, const Configurations& ... config) {
    Strings::get(index, lang);
    (config.ptr(),...);
    (Configurations::length(),...);
    (Configurations::totallength(),...);
}

// "Attaches" a USB device to the USB bus for the lifetime of usbdevice
class usbdevice {
public:
    usbdevice(const usbdevice&) = delete;
    usbdevice(usbdevice&&) = default;
    usbdevice& operator=(const usbdevice&) = delete;
    usbdevice& operator=(usbdevice&&) = default;
    usbdevice(address_with_location addr, const usb1::Device& dev, Strings, const Configurations& ... config)
    : address_ {addr} {
      check_config_count(dev.bNumConfigurations.get(), addr.location);
      usbsys::add(
          usbsys::make_device_info(addr.address, dev.bcdUsb),
          addr.location,
          { dev.ptr(), dev.length() },
          { },
          { {config.ptr(), config.totallength() } ... },
          Strings::get);
    }
    usbdevice(address_with_location addr, const usb2::Device& dev, const usb2::Device_Qualifier& qualifier, Strings,
            const Configurations& ... config)
      : address_ {addr} {
        check_config_count(dev.bNumConfigurations.get(), addr.location);
        usbsys::add(
            usbsys::make_device_info(addr.address, dev.bcdUsb),
            addr.location,
            { dev.ptr(), dev.length() },
            { qualifier.ptr(), qualifier.length() },
            { {config.ptr(), config.totallength() } ... },
            Strings::get);
    }
    ~usbdevice() {
        usbsys::remove(address_.address);
    }
private:
    static void check_config_count(std::size_t count, std::source_location location) {
        if (count != sizeof...(Configurations)) {
            throw std::logic_error{ "Configuration number mismatch:"
                " expected " + std::to_string(count) +
                " provided " + std::to_string(sizeof...(Configurations)) +
                " used at  " + location
            };
        }
    }
    address_with_location address_;
};

using ControlPacket = StandardDeviceRequest;

struct response {
   uint8_t* buffer;
   int& length;
   int& error;
   void send(usb1::DataLength::type atmost, descriptor descr) {
       if (atmost > descr.size()) atmost = static_cast<usb1::DataLength::type>(descr.size());
       std::memcpy(buffer, descr.data(), atmost);
       length += atmost;
   }
   void send(usb1::DataLength::type atmost, const uint8_t descr[]) {
       if (atmost > descr[0]) atmost = descr[0];
       std::memcpy(buffer, descr, atmost);
       length += atmost;
   }
   void send(DeviceStatus_t status) {
       send(static_cast<std::underlying_type_t<DeviceStatus_t>>(status));
   }
   void send(AlternateSetting alternate_setting) {
       send(alternate_setting.get());
   }
   void send(uint16_t value) {
       buffer[0] = static_cast<uint8_t>(value);
       buffer[1] = static_cast<uint8_t>(value >> 8);
       length += 2;
   }
   void send(uint8_t value) {
       buffer[0] = value;
       length += 1;
   }
};

} // namespace ft
} // namespace usbplusplus
