/* Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ut/device.cpp - unit tests for device_descriptor
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "devices.hpp"
#include "ut.hpp"

using namespace usbplusplus;
using namespace usbplusplus::usb2;
using namespace usbplusplus::ut;
using namespace usbplusplus::usb1::tests;
using namespace usbplusplus::usb2::tests;
using namespace boost::ut;

namespace {

namespace expected {
constexpr bytes<18> device_1_00_descriptor {
    0x12, 0x01, 0x00, 0x01, 0x02, 0x07, 0x00, 0x20, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x02, 0x04, 0x01
};

constexpr bytes<18> device_2_00_descriptor {
    0x12, 0x01, 0x00, 0x02, 0x03, 0x01, 0x00, 0x40, 0x02, 0x01, 0x04, 0x03, 0x00, 0x01, 0x01, 0x02, 0x04, 0x02
};

constexpr bytes<10> device_qualifier {
    0x0A, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00
};
} // namespace expected

suite<"Device Descriptor"> device_descriptor_suite = [] {
    "Device Descriptor 1.00"_test = [] {
        expect(eq(TestDevice_1_00, expected::device_1_00_descriptor));
    };
    "Device Descriptor 2.00"_test = [] {
        expect(eq(TestDevice_2_00, expected::device_2_00_descriptor));
    };
    "Device Qualifier"_test = [] {
        expect(eq(TestDeviceQualifier_1_0, expected::device_qualifier));
    };
};

}
