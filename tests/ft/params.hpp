/* Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ft/usbsys.hpp - USB system for testing purpose
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once

#include <cstdint>
#include <cstdio>
#include <string_view>
#include <vector>

#include "iso639.hpp"

static constexpr auto DT_QUALIFIER = libusb_descriptor_type(6); // missing in libusb

struct device_info {
    uint8_t bus;
    uint8_t device;
};

constexpr bool operator==(device_info l, device_info r) {
    return l.bus == r.bus && (l.device == r.device || l.device == 0 || r.device == 0);
}

enum class format_type : int8_t {
    invalid = -1,
    binary,
    decimal,     // 221
    hexadecimal, // DD
    xprefixed,   // 0xDD
    carray,      // { 0xDD }
};

enum class format_case : int8_t {
    lower,
    upper
};

struct format_spec {
    format_type type;
    format_case fmtcase;
};

enum class action_type {
    unspecified,
    invalid,
    list,
    dump,
    help,
};

struct params {
    std::vector<device_info> filter;
    format_spec format;
    action_type action;
    libusb_descriptor_type type;
    uint8_t index;
    uint16_t lang_id;
};


inline uint16_t code_to_lang_id(std::string_view code) {
    if (code.empty()) return 0;
    if (code[0] >= '0' && code[0] <= '9') {
        return static_cast<uint16_t>(std::stoul(std::string{code}, nullptr, 0));
    } else {
        return iso639_to_usb_langid(code);
    }
}

inline format_spec format_char_to_spec(char chr) {
    using namespace std::literals;
    static constexpr auto format_chars = "BbDdHhXxCc"sv;
    const auto pos = format_chars.find(chr);
    if (pos == std::string_view::npos) {
        return { format_type::invalid, {} };
    } else {
        return { format_type(pos / 2), (pos & 1 ? format_case::lower : format_case::upper) };
    }
}

inline libusb_descriptor_type char_to_descr_type(char chr) {
    using namespace std::literals;
    switch(chr) {
    case 'D': return LIBUSB_DT_DEVICE;
    case 'Q': return DT_QUALIFIER;
    case 'C': return LIBUSB_DT_CONFIG;
    case 'S': return LIBUSB_DT_STRING;
    default: return libusb_descriptor_type(0);
    }
}


inline params parse_args(int argc, char *argv[]) {
    using namespace std::literals;
    params result{};
    result.format.type = format_type::decimal;
    if (argc <= 1) {
        result.action = action_type::list;
        return result;
    }
    for(int i = 1; i < argc; ++i) {
        if(argv[i][0] == '-' && argv[i][1] == '-') {
            std::string_view arg { argv[i] };
            if (arg == "--help"sv) {
                result.action = action_type::help;
                break;
            } else if (arg == "--list"sv) {
                if (result.action != action_type::invalid && result.action != action_type::list) {
                    fprintf(stderr, "Conflicting action '%s'\n", arg.data());
                    break;
                }
                result.action = action_type::list;
            } else if (arg == "--dump"sv) {
                if (result.action != action_type::unspecified && result.action != action_type::dump) {
                    fprintf(stderr, "Conflicting action '%s'\n", arg.data());
                    break;
                }
                result.action = action_type::dump;
            } else if (arg.starts_with("--format="sv)) {
                auto format_char = arg["--format="sv.length()];
                result.format = format_char_to_spec(format_char);
                if (result.format.type == format_type::invalid) {
                    fprintf(stderr, "Invalid format specifier '%c'\n", format_char);
                    result.action = action_type::invalid;
                    break;
                }
            } else if (arg.starts_with("--lang="sv)) {
                result.lang_id = code_to_lang_id(arg.substr("--lang="sv.length()));
            } else {
                fprintf(stderr, "Invalid option %s\n", arg.data());
                result.action = action_type::invalid;
                break;
            }
        } else {
            if (result.action == action_type::dump) {
                unsigned bus = 0xFFFFFFFFU, addr = 0xFFFFFFFFU, index = 0xFFFFFFFFU;
                char char_type {};
                auto r = std::sscanf(argv[i], "%u:%u:%c:%u", &bus, &addr, &char_type, &index);
                if (r < 3 || bus > 255 || addr > 255) {
                    fprintf(stderr, "Invalid option %s\n", argv[i]);
                    result.action = action_type::invalid;
                    break;
                }
                result.type = char_to_descr_type(char_type);
                if (result.type == 0) {
                    fprintf(stderr, "Invalid descriptor type '%c'\n", char_type);
                    result.action = action_type::invalid;
                    break;
                } else if (result.type == LIBUSB_DT_DEVICE || result.type == DT_QUALIFIER){
                    if(r == 4) {
                        fprintf(stderr, "Ignoring index for device descriptor in '%s'\n", argv[i]);
                        index = 0;
                    }
                } else {
                    if(r < 4) {
                        fprintf(stderr, "Missing descriptor index in '%s'\n", argv[i]);
                        result.action = action_type::invalid;
                        break;
                    }
                }
                result.filter.push_back({static_cast<uint8_t>(bus), static_cast<uint8_t>(addr)});
                result.index = static_cast<uint8_t>(index);
            } else {
                unsigned bus = 0xFFFFFFFFU, addr = 0;
                char chr {};
                auto r = std::sscanf(argv[i], "%u:%u%c", &bus, &addr, &chr);
                if (r < 1 || r > 2 || bus > 255 || addr > 255) {
                    fprintf(stderr, "Invalid option %s\n", argv[i]);
                    result.action = action_type::invalid;
                    break;
                }
                result.filter.push_back({static_cast<uint8_t>(bus), static_cast<uint8_t>(addr)});
                result.action = action_type::list;
            }
        }
    }
    return result;
}

 // namespace 
