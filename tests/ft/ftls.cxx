
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wzero-length-array"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#include <libusb.h>
#include <unistd.h>
#pragma GCC diagnostic pop
#include <cstring>
#include <algorithm>
#include <string_view>
#include <span>
#include <vector>

#include <utf8.hpp>
#include "params.hpp"

static constexpr int timeout = 5000; // 5ms

static int print_error(libusb_device* dev, const char message[]) {
    fprintf(stderr, "%s %d:%d\n", message, libusb_get_bus_number(dev), libusb_get_device_address(dev));
    return 1;
}

class device_handle {
public:
    device_handle(libusb_device* dev) {
        if (libusb_open(dev, &handle_) < 0) {
            print_error(dev, "Failed to open device");
        }
    }
    device_handle(device_handle&&) = default;
    device_handle(const device_handle&) = delete;
    ~device_handle() {
        if (handle_ != nullptr)
            libusb_close(handle_);
    }
    device_handle& operator=(const device_handle&) = delete;
    device_handle& operator=(device_handle&&) = delete;
    operator libusb_device_handle*() const { return handle_; }
private:
    libusb_device_handle* handle_ {};
};


static void print_string(libusb_device_handle *handle, uint16_t langid, uint8_t index, const char label[]) {
    unsigned char string_descriptor[256] = {0};
    if (libusb_get_string_descriptor(handle, index, langid, string_descriptor, sizeof(string_descriptor)) > 0) {
        if(string_descriptor[0] < 4 || string_descriptor[0] & 1 || string_descriptor[1] != LIBUSB_DT_STRING) {
            fprintf(stderr, "Got invalid string descriptor {%X,%X,...}\n", string_descriptor[0], string_descriptor[1]);
            return;
        }
        std::u16string_view uv(reinterpret_cast<const char16_t*>(&string_descriptor[2]),
                               static_cast<std::size_t>(string_descriptor[0] / 2U));
        auto str = utf8(uv);
        printf(" %s: '%s'", label, str.c_str());
    }
}

static bool pass_filter(device_info info, std::vector<device_info> filter) {
    if (filter.empty()) return true;
    const auto found = std::find(filter.begin(), filter.end(), info);
    return found != filter.end();
}

static int list_devices(libusb_device **devs, const params& args) {
    libusb_device *dev;
    uint8_t path[8];
    int i = 0;

    while ((dev = devs[i++]) != NULL) {
        libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            print_error(dev, "Failed to get device descriptor");
            return 1;
        }
        auto busnum = libusb_get_bus_number(dev);
        auto addr = libusb_get_device_address(dev);

        if (!pass_filter({busnum, addr}, args.filter))
            continue;
        printf("%d:%d ID [%04x:%04x]", busnum, addr, desc.idVendor, desc.idProduct);

        r = libusb_get_port_numbers(dev, path, sizeof(path));
        if (r > 0) {
            printf(" Port: %d", path[0]);
            for (int j = 1; j < r; j++)
                printf(".%d", path[j]);
        }
        device_handle handle(dev);
        if (handle != nullptr) {
            print_string(handle, args.lang_id, desc.iManufacturer, "Manufacturer");
            print_string(handle, args.lang_id, desc.iProduct, "Product");
            print_string(handle, args.lang_id, desc.iSerialNumber, "Serial Number");
        }
        printf("\n");
    }
    return 0;
}

static libusb_device* find_device(libusb_device **devs, const device_info di) {
    libusb_device *dev;
    int i = 0;
    while ((dev = devs[i++]) != NULL) {
        libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) continue;
        if (di.bus == libusb_get_bus_number(dev) && di.device == libusb_get_device_address(dev))
          return dev;
    }
    return nullptr;
}

static int dump_descriptor_data(std::span<const uint8_t> data, format_spec fmt) {
    if(fmt.type == format_type::binary) {
        if (write(1, data.data(), data.size()) < 0) {
            fprintf(stderr, "Error %d writing to stdout: %s", errno, strerror(errno));
            return 1;
        }
    } else {
        static const char * const formats[2][4] = {
                { "%s%3d", "%s%2.2x", "%s0x%2.2x", "%s0x%x"},
                { "%s%3d", "%s%2.2X", "%s0x%2.2X", "%s0x%X"}};
        static const char * const delims[2][4] = {{"", " ", ""}, {"{", ", ", "};"}};
        const char* fstr = formats[fmt.fmtcase == format_case::upper][static_cast<int>(fmt.type) - 1];
        const char * const * delim = delims[fmt.type == format_type::carray];
        const char *dlm = delim[0];
        for(auto chr : data) {
            printf(fstr, dlm, chr);
            dlm = delim[1];
        }
        puts(delim[2]);
    }
    return 0;
}

static int dump_device_descriptor(libusb_device* dev, format_spec fmt) {
    libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
        return print_error(dev, "Failed to get device descriptor");
    }
    return dump_descriptor_data({&desc.bLength, sizeof(desc)}, fmt);
}

static int dump_config_descriptor(libusb_device* dev, uint8_t index, format_spec fmt) {
    device_handle handle {dev};
    unsigned char data[2048] = {0};
    int r = libusb_control_transfer(handle, LIBUSB_ENDPOINT_IN,
        LIBUSB_REQUEST_GET_DESCRIPTOR, static_cast<uint16_t>(LIBUSB_DT_CONFIG << 8) | index, 0,
        data, sizeof(data), timeout);
    if (r < 0) {
        fprintf(stderr, "Failed to get config descriptor %d ", index);
        return print_error(dev, "from device");
    }
    r = dump_descriptor_data({data, data + r}, fmt);
    return r;
}

static int dump_string_descriptor(libusb_device* dev, uint8_t index, uint16_t langid, format_spec fmt) {
    device_handle handle{dev};
    unsigned char string_descriptor[256] = {0};
    if (libusb_get_string_descriptor(handle, index, langid, string_descriptor, sizeof(string_descriptor)) > 0) {
        return dump_descriptor_data({string_descriptor, string_descriptor[0]}, fmt);
    } else {
        fprintf(stderr, "Failed to get string descriptor %d ", index);
        return print_error(dev, "from device");
    }
}

static int dump_qualifier_descriptor(libusb_device* dev, format_spec fmt) {
    device_handle handle {dev};
    unsigned char buf[10];
    int r = libusb_control_transfer(handle,
            static_cast<uint8_t>(LIBUSB_ENDPOINT_IN) |
            static_cast<uint8_t>(LIBUSB_REQUEST_TYPE_STANDARD) |
            static_cast<uint8_t>(LIBUSB_RECIPIENT_DEVICE),
            LIBUSB_REQUEST_GET_DESCRIPTOR,
            DT_QUALIFIER << 8, 0, buf, sizeof(buf), timeout);
    if (r < 0) {
        return print_error(dev, "Failed to get device qualifier from device");
    } else {
        return dump_descriptor_data({buf, sizeof(buf)}, fmt);
    }
}

static int dump_descriptor(libusb_device **devs, const params& args) {
    if (args.filter.empty()) {
        fprintf(stderr, "Missing device argument\n");
        return 2;
    }
    libusb_device *dev = find_device(devs, args.filter[0]);
    if (dev == nullptr) {
        fprintf(stderr, "No such device %d:%d\n", args.filter[0].bus, args.filter[0].device);
        return 1;
    }

    switch(static_cast<unsigned>(args.type)) {
    case LIBUSB_DT_DEVICE:
        return dump_device_descriptor(dev, args.format);
    case LIBUSB_DT_CONFIG:
        return dump_config_descriptor(dev, args.index, args.format);
    case LIBUSB_DT_STRING:
        return dump_string_descriptor(dev, args.index, args.lang_id, args.format);
    case DT_QUALIFIER:
        return dump_qualifier_descriptor(dev, args.format);
    default:
        fprintf(stderr, "Unsupported descriptor type: %d\n", args.type);
    }
    return 2;
}

static void print_help() {
    printf("Usage:\n"
"  ftls [--list] [--lang=<lang>]  [<bus>[:<addr>]]...\n\t\tLists devices matching by bus and addr\n"
"  ftls --dump [--lang=<lang>] [--format=<fmt>] <bus>:<addr>:<descr>[:<index>]\n\t\tDumps descriptor\n"
"  ftls --help\n\t\tPrints this help string\n"
"Where:\n"
"  <bus>   - bus number, 1..255\n"
"  <addr>  - device address, 1..255\n"
"  <descr> - descriptor type, D - device, Q - device qualifier, C - config, S - string\n"
"  <index> - descriptor index, 0..255\n"
"  <fmt>   - dump format, B - binary, C C-array, D - decimal, H - hexadecimal, X - with prefix 0x \n"
"  <lang>  - language code (ISO 639) or hexadecimal language ID\n"
    );
}

int main(int argc, char *argv[]) {
    auto args = parse_args(argc, argv);
    if (args.action == action_type::invalid) {
        return 2;
    }
    if (args.action == action_type::help) {
        print_help();
        return 0;
    }
    libusb_device **devs;
    int r;
    ssize_t cnt;

    r = libusb_init_context(nullptr, nullptr, 0);
    if (r < 0)
        return 1;

    cnt = libusb_get_device_list(nullptr, &devs);
    if (cnt < 0){
        libusb_exit(nullptr);
        return 1;
    }
    int retcode;
    if (args.action == action_type::dump) {
        retcode = dump_descriptor(devs, args);
    } else {
        retcode = list_devices(devs, args);
    }
    libusb_free_device_list(devs, 1);

    libusb_exit(nullptr);
    return retcode;
}

