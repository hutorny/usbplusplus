/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ut/strings.cpp - unit tests for strings
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once
#include <cstdint>
#include <string>
#include <string_view>

inline std::string utf8(char16_t chr) {
    auto code = static_cast<std::uint16_t>(chr);
    static constexpr auto c = [](unsigned sign, unsigned cod) { return static_cast<char>(sign | (cod & 0x3FU)); };

    if (code < 0x80U) {
        return { static_cast<char>(code) };
    } else if (code < 0x800U) {
        return { c(0xC0U, code >> 6), c(0x80U, code) };
    } else {
        return { c (0xE0U, code >> 12), c(0x80U, code >> 6), c(0x80U, code) };
    }
}

inline std::string utf8(std::u16string_view str) {
    std::string result {};
    result.reserve(str.size());
    for(auto c : str) result += utf8(c);
    return result;
}
