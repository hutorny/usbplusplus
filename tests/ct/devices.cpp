/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ct/devices.cpp - compile time tests for device descriptor
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "devices.hpp"

namespace usbplusplus {
namespace usb2 {
namespace tests {
using namespace usbplusplus::usb1::tests;

static_assert(TestDevice_1_00.length() == 18, "TestDevice_1_00.length()");
static_assert(TestDevice_1_00.descriptortype() == DescriptorType_t::DEVICE, "");
static_assert(TestDevice_2_00.length() == 18, "TestDevice_2_00.length()");
static_assert(TestDevice_2_00.descriptortype() == DescriptorType_t::DEVICE, "TestDevice_2_00.descriptortype()");
static_assert(TestDeviceQualifier_1_0.descriptortype() == DescriptorType_t::DEVICE_QUALIFIER, "TestDeviceQualifier_1_0.descriptortype()");
static_assert(TestDeviceQualifier_1_0.length() == 10, "TestDeviceQualifier_1_0.length()");

} // namespace tests
} // namespace usb2
} // namespace usbplusplus
