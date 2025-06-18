/* Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ut/ut.hpp - USB++ unit testing utility classes and functions
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once
#include "boost/ut.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <iostream>
#include <iomanip>

#include "utf8.hpp"

namespace usbplusplus {
namespace ut {
template<std::size_t N>
using bytes = std::array<uint8_t, N>;

template<class Descriptor>
requires requires(const Descriptor& descriptor) { descriptor.length(); }
// returns totallength() if available and length() otherwise
std::size_t length_of(const Descriptor& descriptor) {
    if constexpr(requires {descriptor.totallength();}) {
        return descriptor.totallength();
    } else {
        return descriptor.length();
    }
}

template<class Descriptor, class ExpectedData>
requires requires(const Descriptor& descriptor, const ExpectedData& data) {
    descriptor.ptr(); descriptor.length(); data.size(); data.begin(); }
bool eq(const Descriptor& descriptor, const ExpectedData& data,
        std::source_location sl = std::source_location::current()) {
    if( !(boost::ut::expect(boost::ut::eq(length_of(descriptor), data.size()))
                            << "(length) ") )
        return false;
    for(auto ptr = descriptor.ptr(), i = 0U; i < data.size(); i++) {
        if(!(boost::ut::expect(boost::ut::eq(static_cast<unsigned>(data[i]), static_cast<unsigned>(ptr[i])), sl)
                               << "( @" << i << ") "))
            return false;
    }
    return true;
}

template<class ExpectedData>
requires requires(const ExpectedData& data) { data.size(); data.begin(); }
bool eq(const std::uint8_t actual[], const ExpectedData& data,
        std::source_location sl = std::source_location::current()) {
    const std::size_t length = actual[0];
    if( !(boost::ut::expect(boost::ut::eq(static_cast<std::size_t>(length), data.size()), sl)
                            << "(length) ") )
        return false;
    for(auto i = 0U; i < data.size(); i++) {
        if(!(boost::ut::expect(boost::ut::eq(static_cast<unsigned>(data[i]), static_cast<unsigned>(actual[i])), sl)
                               << "( @" << i << ") "))
            return false;
    }
    return true;
}

template<std::size_t Length>
bool eq(const std::uint8_t actual[], const char16_t (&expected)[Length],
        std::source_location sl = std::source_location::current()) {
    using traits = std::char_traits<char16_t>;
	const char16_t* str = reinterpret_cast<const char16_t*>(&actual[2]);
	const std::size_t length = actual[0]/2-1;
	if( !(boost::ut::expect(boost::ut::eq(length, Length-1), sl) << "(length) "))
		return false;
	if( !(boost::ut::expect(traits::compare(str, expected, Length-1) == 0, sl) << std::setw(1)
	        << "["  << ("'" + utf8({str, length}) + "'")
	        << "==" << ("'" + utf8(expected) + "'") << "]"))
		return false;
	return true;
}

}
} // namespace
