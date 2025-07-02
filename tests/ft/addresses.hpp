/* Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ft/ft.hpp - USB++ functional testing utility classes and functions
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once

#include <cstdint>

namespace usbplusplus {
namespace ft {

/// device addresses for USB functional tests
enum class devaddr : uint8_t {
    local, // preferred for locally scoped devices
    uac1,
    uac2,
    cdc,
    hid,
    test1 = 0x20,
    test2,
};

} // namespace ft
} // namespace usbplusplus
