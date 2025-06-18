/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * test/common/strings.hpp - commonly used strings
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once
#include <usbplusplus/usbplusplus.hpp>

#include "strings.hpp"

namespace usbplusplus {
namespace usb1 {
namespace tests {

constexpr const Device TestDevice_1_00 = {
    .bLength = {},
    .bDescriptorType = {},
    .bcdUsb = 1.00_bcd,
    .bDeviceClass = DeviceClass::CDC,
    .bDeviceSubClass = 0x7,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = MaxPacketSize0_t::_32,
    .idVendor = 0x0101,
    .idProduct = 0x0101,
    .bcdDevice = 1.00_bcd,
    .iManufacturer = TestStrings::indexof(sManufacturer),
    .iProduct = TestStrings::indexof(sProduct),
    .iSerialNumber = TestStrings::indexof(sSerialNumber),
    .bNumConfigurations = 1
};

} // namespace tests
} // namespace usb1

namespace usb2 {
namespace tests {
using namespace usb1::tests;

constexpr const Device TestDevice_2_00 = {
    .bLength = {},
    .bDescriptorType = {},
    .bcdUsb = 2.00_bcd,
    .bDeviceClass = DeviceClass::HID,
    .bDeviceSubClass = 0x1,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = MaxPacketSize0_t::_64,
    .idVendor = 0x0102,
    .idProduct = 0x0304,
    .bcdDevice = 1.00_bcd,
    .iManufacturer = TestStrings::indexof(sManufacturer),
    .iProduct = TestStrings::indexof(sProduct),
    .iSerialNumber = TestStrings::indexof(sSerialNumber),
    .bNumConfigurations = 2
};

constexpr const  Device_Qualifier TestDeviceQualifier_1_0 = {
    {},
    DescriptorType<Device_Qualifier>(),
    1.0_bcd,
    DeviceClass::Defined_in_the_Interface_Descriptors,
    DeviceSubClass(0),
    DeviceProtocol(0),
    MaxPacketSize0_t::_16,
    NumConfigurations(1),
    Reserved<1>()
};


} // namespace tests
} // namespace usb2
} // namespace usbplusplus
