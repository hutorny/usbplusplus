/* Copyright (C) 2018 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * ustring.hpp - UTF16-LE compile time and run-time strings
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
#include "byteorder.hpp"
#include <utility>

/**
 * Compile time string operations and  lists of strings
 */

namespace usbplusplus {
constexpr unsigned ustring_size = 64; /* increasing this requires adding
										explicit specializations to utf16le */

/** Simple unicode string for easy of use									*/
using ustring = const char16_t[ustring_size];

inline constexpr unsigned length_impl(ustring s, unsigned pos) {
	return (pos < ustring_size && s[pos]) ? length_impl(s, pos+1) : pos;
}
inline constexpr unsigned length(ustring s) {
	return length_impl(s,0);
}

inline constexpr bool equal_impl(ustring a, ustring b, unsigned pos) {
	return  ( pos < ustring_size && a[pos] == b[pos] )
			? equal_impl(a,b,pos+1)
			: pos == ustring_size;
}

inline constexpr bool equal(ustring a, ustring b) {
	return equal_impl(a,b,0);
}

/** UTF-16LE packed string of known length N								*/
template<unsigned N>
struct utf16le {
	using type = char16_t[N];
	const type value;
	constexpr utf16le(ustring src) : utf16le{src, std::make_index_sequence<N>()} {
		static_assert(N < sizeof(ustring),"String is too long");
	}
private:
	template<std::size_t ... I>
	constexpr utf16le(ustring src, std::index_sequence<I...>) : value { byteorder<>::le(src[I]) ... } { }
};

/** converts char16_t[64] to string of necessary length						*/
template<ustring String>
struct cstring {
	/* static constexpr ustring value = String; // does not work */
	static constexpr unsigned length = usbplusplus::length(String);
	static constexpr utf16le<length> string = {String};
};

/* storage allocation														*/
template<ustring String>
constexpr utf16le<cstring<String>::length> cstring<String>::string;

/** List of strings															*/
template<ustring ... List>
struct list;

template<ustring String>
struct list<String> {
	static constexpr unsigned indexof(ustring another) {
		return equal(String, another) ? 1 : 0;
	}
};

inline constexpr unsigned incifnz(unsigned v) { return v ? v + 1 : v; }

template<ustring String, ustring ... List>
struct list<String, List...> {
	static constexpr unsigned indexof(ustring another) {
		return equal(String, another)
			? 1 : incifnz(list<List...>::indexof(another));
	}
};
}

