/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ct/cdc.cpp - compile tests for CDC
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include <usbplusplus/usbplusplus.hpp>
#include <usbplusplus/cdc.hpp>

namespace usbplusplus {
namespace cdc {
namespace tests {

static_assert(CdcEcmControl::length() == 9, "CdcEcmControl::length()");
static_assert(CdcAcmControl::length() == 9, "CdcEcmControl::length()");

} // namespace tests
} // namespace cdc
} // namespace usbplusplus
