/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ut/configurations.cpp - unit tests for configuration descriptors
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "configurations.hpp"
#include "ut.hpp"

using namespace usbplusplus;
using namespace usbplusplus::usb2;
using namespace usbplusplus::ut;
using namespace usbplusplus::usb2::tests;
using namespace boost::ut;

namespace {

namespace expected {
constexpr bytes<41> uac2_configuration1 {
 0x09, 0x02, 0x29, 0x00, 0x02, 0x01, 0x00, 0xA0, 0x32, 0x09, 0x04, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x07, 0x05,
 0x80, 0x01, 0x00, 0x01, 0x01, 0x09, 0x04, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x07, 0x05, 0x01, 0x01, 0x00, 0x01,
 0x01
};

constexpr bytes<55> uac2_configuration2 {
 0x09, 0x02, 0x37, 0x00, 0x02, 0x02, 0x00, 0x80, 0x32, 0x09, 0x04, 0x01, 0x01, 0x02, 0x01, 0x00, 0x20, 0x00, 0x07, 0x05,
 0x81, 0x01, 0x00, 0x01, 0x01, 0x07, 0x05, 0x02, 0x01, 0x00, 0x01, 0x01, 0x09, 0x04, 0x02, 0x01, 0x02, 0x01, 0x00, 0x20,
 0x00, 0x07, 0x05, 0x83, 0x01, 0x00, 0x01, 0x01, 0x07, 0x05, 0x04, 0x01, 0x00, 0x01, 0x01
};

constexpr bytes<71> uac2_configuration3 {
 0x09, 0x02, 0x47, 0x00, 0x03, 0x03, 0x00, 0x80, 0x32, 0x09, 0x04, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x07, 0x05,
 0x80, 0x01, 0x00, 0x01, 0x01, 0x09, 0x04, 0x02, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x07, 0x05, 0x81, 0x01, 0x00, 0x01,
 0x01, 0x07, 0x05, 0x02, 0x01, 0x00, 0x01, 0x01, 0x09, 0x04, 0x03, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x07, 0x05, 0x83,
 0x01, 0x00, 0x01, 0x01, 0x07, 0x05, 0x04, 0x01, 0x00, 0x01, 0x01
};

} // namespace expected

suite<"Configuration Descriptor"> configuration_descriptor_suite = [] {
    "UAC2 Configuration1 Descriptor"_test = [] {
        expect(eq(TestUAC2Configuration_1, expected::uac2_configuration1));
    };
    "UAC2 Configuration2 Descriptor"_test = [] {
        expect(eq(TestUAC2Configuration_2, expected::uac2_configuration2));

    };
    "UAC3 Configuration3 Descriptor"_test = [] {
        expect(eq(TestUAC2Configuration_3, expected::uac2_configuration3));
    };
};
} // namespace



