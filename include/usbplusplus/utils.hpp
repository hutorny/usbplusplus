/* Copyright (C) 2025 Michiel van Leeuwen <michiel@embeddedacoustics.com>
 *                    Eugene Hutorny <eugene@hutorny.in.ua>
 * utils.hpp - Helpers
 *
 * This file is a part of USB++ library
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include <type_traits>

namespace usbplusplus {

template<typename Lhs, typename Rhs = Lhs>
constexpr bool enable_or = false;
template<typename Lhs, typename Rhs = Lhs>
constexpr bool enable_and = false;

// Enable OR of the items from the same Enum into the same Enum. Must be enabled explitly
template <typename Lhs, typename Rhs>
constexpr std::enable_if_t<enable_or<Lhs, Rhs>, Lhs>
operator|(Lhs lhs, Rhs rhs) {
    using UL = std::underlying_type_t<Lhs>;
    using UR = std::underlying_type_t<Lhs>;
    return static_cast<Lhs>(static_cast<UL>(lhs) | static_cast<UR>(rhs));
}

// Enable AND of the items from the same Enum into the same Enum. Must be enabled explitly
template <typename Lhs, typename Rhs>
constexpr std::enable_if_t<enable_and<Lhs, Rhs>, Lhs>
operator&(Lhs lhs, Rhs rhs) {
    using UL = std::underlying_type_t<Lhs>;
    using UR = std::underlying_type_t<Lhs>;
    return static_cast<Lhs>(static_cast<UL>(lhs) & static_cast<UR>(rhs));
}

} // namespace usbplusplus
