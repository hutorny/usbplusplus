/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ct/configurations.cpp - compile time tests for configuration descriptors
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "configurations.hpp"

namespace usbplusplus {
namespace usb2 {
namespace tests {

static_assert(TestUAC2Configuration_1.length() == 9, "TestUAC2Configuration_1.length()");
static_assert(TestUAC2Configuration_1.totallength() == 27, "TestUAC2Configuration_1.totallength()");
static_assert(TestUAC2Configuration_1.descriptortype() == DescriptorType_t::CONFIGURATION, "TestUAC2Configuration_1.descriptortype()");
static_assert(TestUAC2Configuration_2.length() == 9, "TestUAC2Configuration_2.length()");
static_assert(TestUAC2Configuration_2.totallength() == 55, "TestUAC2Configuration_2.totallength()");
static_assert(TestUAC2Configuration_2.descriptortype() == DescriptorType_t::CONFIGURATION, "TestUAC2Configuration_2.descriptortype()");
static_assert(TestUAC2Configuration_3.length() == 9, "TestUAC2Configuration_3.length()");
static_assert(TestUAC2Configuration_3.totallength() == 64, "TestUAC2Configuration_3.totallength()");
static_assert(TestUAC2Configuration_3.descriptortype() == DescriptorType_t::CONFIGURATION, "TestUAC2Configuration_3.descriptortype()");

} // namespace tests
} // namespace usb2
} // namespace usbplusplus



