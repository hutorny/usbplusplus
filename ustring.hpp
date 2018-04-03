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
	constexpr utf16le(ustring src) : value { } {
		static_assert(N < sizeof(ustring),"String is too long");
	}
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

/* Is there a better way for copying and LE-transforming strings?			*/
template<> struct utf16le<1> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[1];
	constexpr utf16le(ustring src) :
			value { _(src[0]) } {
	}
};
template<> struct utf16le<2> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[2];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]) } {
	}
};
template<> struct utf16le<3> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[3];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]) } {
	}
};
template<> struct utf16le<4> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[4];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]) } {
	}
};
template<> struct utf16le<5> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[5];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]) } {
	}
};
template<> struct utf16le<6> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[6];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]) } {
	}
};
template<> struct utf16le<7> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[7];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]) } {
	}
};
template<> struct utf16le<8> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[8];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]) } {
	}
};
template<> struct utf16le<9> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[9];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]) } {
	}
};
template<> struct utf16le<10> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[10];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]) } {
	}
};
template<> struct utf16le<11> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[11];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]) } {
	}
};
template<> struct utf16le<12> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[12];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]) } {
	}
};
template<> struct utf16le<13> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[13];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]) } {
	}
};
template<> struct utf16le<14> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[14];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]) } {
	}
};
template<> struct utf16le<15> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[15];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]) } {
	}
};
template<> struct utf16le<16> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[16];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]) } {
	}
};
template<> struct utf16le<17> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[17];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]) } {
	}
};
template<> struct utf16le<18> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[18];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]) } {
	}
};
template<> struct utf16le<19> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[19];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]) } {
	}
};
template<> struct utf16le<20> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[20];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]) } {
	}
};
template<> struct utf16le<21> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[21];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]) } {
	}
};
template<> struct utf16le<22> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[22];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]) } {
	}
};
template<> struct utf16le<23> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[23];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]) } {
	}
};
template<> struct utf16le<24> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[24];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]) } {
	}
};
template<> struct utf16le<25> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[25];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]) } {
	}
};
template<> struct utf16le<26> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[26];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]) } {
	}
};
template<> struct utf16le<27> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[27];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]) } {
	}
};
template<> struct utf16le<28> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[28];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]) } {
	}
};
template<> struct utf16le<29> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[29];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]) } {
	}
};
template<> struct utf16le<30> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[30];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]) } {
	}
};
template<> struct utf16le<31> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[31];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]) } {
	}
};
template<> struct utf16le<32> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[32];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]) } {
	}
};
template<> struct utf16le<33> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[33];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]) } {
	}
};
template<> struct utf16le<34> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[34];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]) } {
	}
};
template<> struct utf16le<35> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[35];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]) } {
	}
};
template<> struct utf16le<36> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[36];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]) } {
	}
};
template<> struct utf16le<37> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[37];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]) } {
	}
};
template<> struct utf16le<38> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[38];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]) } {
	}
};
template<> struct utf16le<39> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[39];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]) } {
	}
};
template<> struct utf16le<40> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[40];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]) } {
	}
};
template<> struct utf16le<41> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[41];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]) } {
	}
};
template<> struct utf16le<42> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[42];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]) } {
	}
};
template<> struct utf16le<43> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[43];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]) } {
	}
};
template<> struct utf16le<44> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[44];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]) } {
	}
};
template<> struct utf16le<45> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[45];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]) } {
	}
};
template<> struct utf16le<46> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[46];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]) } {
	}
};
template<> struct utf16le<47> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[47];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]) } {
	}
};
template<> struct utf16le<48> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[48];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]) } {
	}
};
template<> struct utf16le<49> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[49];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]) } {
	}
};
template<> struct utf16le<50> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[50];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]) } {
	}
};
template<> struct utf16le<51> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[51];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]) } {
	}
};
template<> struct utf16le<52> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[52];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]) } {
	}
};
template<> struct utf16le<53> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[53];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]) } {
	}
};
template<> struct utf16le<54> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[54];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]) } {
	}
};
template<> struct utf16le<55> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[55];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]), _(src[54]) } {
	}
};
template<> struct utf16le<56> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[56];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]), _(src[54]), _(
					src[55]) } {
	}
};
template<> struct utf16le<57> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[57];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]), _(src[54]), _(
					src[55]), _(src[56]), } {
	}
};
template<> struct utf16le<58> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[58];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]), _(src[54]), _(
					src[55]), _(src[56]), _(src[57]) } {
	}
};
template<> struct utf16le<59> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[59];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]), _(src[54]), _(
					src[55]), _(src[56]), _(src[57]), _(src[58]) } {
	}
};
template<> struct utf16le<60> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[60];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]), _(src[54]), _(
					src[55]), _(src[56]), _(src[57]), _(src[58]), _(src[59]) } {
	}
};
template<> struct utf16le<61> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[61];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]), _(src[54]), _(
					src[55]), _(src[56]), _(src[57]), _(src[58]), _(src[59]), _(
					src[60]) } {
	}
};
template<> struct utf16le<62> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[62];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]), _(src[54]), _(
					src[55]), _(src[56]), _(src[57]), _(src[58]), _(src[59]), _(
					src[60]), _(src[61]) } {
	}
};
template<> struct utf16le<63> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[63];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]), _(src[54]), _(
					src[55]), _(src[56]), _(src[57]), _(src[58]), _(src[59]), _(
					src[60]), _(src[61]), _(src[62]) } {
	}
};
template<> struct utf16le<64> {
	static constexpr char16_t _(char16_t chr) {
		return byteorder<>::le(chr);
	}
	const char16_t value[64];
	constexpr utf16le(ustring src) :
			value { _(src[0]), _(src[1]), _(src[2]), _(src[3]), _(src[4]), _(
					src[5]), _(src[6]), _(src[7]), _(src[8]), _(src[9]), _(
					src[10]), _(src[11]), _(src[12]), _(src[13]), _(src[14]), _(
					src[15]), _(src[16]), _(src[17]), _(src[18]), _(src[19]), _(
					src[20]), _(src[21]), _(src[22]), _(src[23]), _(src[24]), _(
					src[25]), _(src[26]), _(src[27]), _(src[28]), _(src[29]), _(
					src[30]), _(src[31]), _(src[32]), _(src[33]), _(src[34]), _(
					src[35]), _(src[36]), _(src[37]), _(src[38]), _(src[39]), _(
					src[40]), _(src[41]), _(src[42]), _(src[43]), _(src[44]), _(
					src[45]), _(src[46]), _(src[47]), _(src[48]), _(src[49]), _(
					src[50]), _(src[51]), _(src[52]), _(src[53]), _(src[54]), _(
					src[55]), _(src[56]), _(src[57]), _(src[58]), _(src[59]), _(
					src[60]), _(src[61]), _(src[62]), _(src[63]) } {
	}
};

}
