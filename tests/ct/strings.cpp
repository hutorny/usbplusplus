/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ct/strings.cpp - compile time tests for strings
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */


#include "strings.hpp"

namespace usbplusplus {
namespace usb1 {
namespace tests {

static_assert(TestStrings::indexof(sManufacturer) == 1, "TestStrings::indexof(sManufacturer)");
static_assert(TestStrings::indexof(sProduct) == 2, "TestStrings::indexof(sProduct)");
static_assert(TestStrings::indexof(sInterface) == 3, "TestStrings::indexof(sInterface)");
static_assert(TestStrings::indexof(sSerialNumber) == 4, "TestStrings::indexof(sSerialNumber)");
static_assert(TestStrings::indexof(uProduct) == 0, "TestStrings::indexof(uProduct)");

static_assert(TestMultiStrings::indexof(sManufacturer) == 1, "TestMultiStrings::indexof(sManufacturer)");
static_assert(TestMultiStrings::indexof(sProduct) == 2, "TestMultiStrings::indexof(sProduct)");
static_assert(TestMultiStrings::indexof(sInterface) == 3, "TestMultiStrings::indexof(sInterface)");
static_assert(TestMultiStrings::indexof(sSerialNumber) == 0, "TestMultiStrings::indexof(sSerialNumber)");
static_assert(TestMultiStrings::indexof(uProduct) == 0, "TestMultiStrings::indexof(uProduct)");

} // namespace tests
} // namespace usb1
} // namespace usbplusplus

