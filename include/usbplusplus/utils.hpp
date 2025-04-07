/* Copyright (C) 2018 Michiel van Leeuwen <michiel@embeddedacoustics.com>
 *
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

namespace usbplusplus
{

// Enable OR of two different enums into one bitmask
template <typename T, typename U, std::enable_if_t<std::is_same_v<std::underlying_type_t<T>, std::underlying_type_t<U>>, bool> = true>
constexpr auto operator|(T left, U right) -> std::underlying_type_t<T>
{
    using Type = std::underlying_type_t<T>;
    return static_cast<Type>(left) | static_cast<Type>(right);
}

template <typename T, typename U, std::enable_if_t<std::is_same_v<T, std::underlying_type_t<U>>, bool> = true>
constexpr auto operator|(T left, U right) -> T
{
    return left | static_cast<T>(right);
}

template <typename T, typename U, std::enable_if_t<std::is_same_v<std::underlying_type_t<T>, U>, bool> = true>
constexpr auto operator|(T left, U right) -> U
{
    return static_cast<U>(left) | right;
}

// Enable AND of two different enums into one bitmask
template <typename T, typename U, std::enable_if_t<std::is_same_v<std::underlying_type_t<T>, std::underlying_type_t<U>>, bool> = true>
constexpr auto operator&(T left, U right) -> std::underlying_type_t<T>
{
    using Type = std::underlying_type_t<T>;
    return static_cast<Type>(left) & static_cast<Type>(right);
}

template <typename T, typename U, std::enable_if_t<std::is_same_v<T, std::underlying_type_t<U>>, bool> = true>
constexpr auto operator&(T left, U right) -> T
{
    return left & static_cast<T>(right);
}

template <typename T, typename U, std::enable_if_t<std::is_same_v<std::underlying_type_t<T>, U>, bool> = true>
constexpr auto operator&(T left, U right) -> U
{
    return static_cast<U>(left) & right;
}

// Enable OR of the items from the same Enum into one bitmask
#define DEFINE_ENUM_CLASS_OR(Enum)                       \
inline constexpr Enum operator|(Enum Lhs, Enum Rhs)      \
{                                                        \
    return static_cast<Enum>(                            \
        static_cast<std::underlying_type_t<Enum>>(Lhs) | \
        static_cast<std::underlying_type_t<Enum>>(Rhs)); \
}

// Enable OR of the items from the same Enum into one bitmask
#define DEFINE_ENUM_CLASS_AND(Enum)                       \
inline constexpr Enum operator&(Enum Lhs, Enum Rhs)      \
{                                                        \
    return static_cast<Enum>(                            \
        static_cast<std::underlying_type_t<Enum>>(Lhs) & \
        static_cast<std::underlying_type_t<Enum>>(Rhs)); \
}

}
