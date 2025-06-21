/* Copyright (C) 2018 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * byteorder.hpp - Byte order and endianess utilities
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
#include <cstdint>
namespace usbplusplus {
//std::endian was not available at creation time
enum class endian {
	little = __ORDER_LITTLE_ENDIAN__,
	big    = __ORDER_BIG_ENDIAN__,
	native = __BYTE_ORDER__
};

namespace detail {
template<unsigned N_bytes, typename = signed> struct intN_t;
template<> struct intN_t<0, unsigned> { typedef uint8_t  type; };
template<> struct intN_t<0, signed  > { typedef int8_t   type; };
template<> struct intN_t<1, unsigned> { typedef uint8_t  type; };
template<> struct intN_t<1, signed  > { typedef int8_t   type; };
template<> struct intN_t<2, unsigned> { typedef uint16_t type; };
template<> struct intN_t<2, signed  > { typedef int16_t  type; };
template<> struct intN_t<4, unsigned> { typedef uint32_t type; };
template<> struct intN_t<4, signed  > { typedef int32_t  type; };
template<unsigned bytes> struct uintN_t : intN_t<bytes, unsigned> {};
}

template<endian = endian::native>
struct byteorder;

template<>
struct byteorder<endian::little> {
	/** Native to little-endian conversion */
	template<typename T>
	static constexpr T le(T v) { return  v; }
	/** Little-endian to native conversion */
	template<typename T>
	static constexpr T ne(T v) { return  v; }
};

template<>
struct byteorder<endian::big> {
	/** Native to little-endian conversion */
	template<typename T>
	static constexpr T le(T v) {
		constexpr unsigned size = sizeof(T);
		using type = typename detail::intN_t<size, unsigned>::type;
		return static_cast<T>(le<type>(static_cast<type>(v)));
	}
	/** Little-endian to native conversion */
	template<typename T>
	static constexpr T ne(T v) { return le<T>(v); }
};

template<>
inline constexpr uint8_t byteorder<endian::big>::le<uint8_t>(uint8_t v) {
	return v;
}

template<>
inline constexpr int8_t byteorder<endian::big>::le<int8_t>(int8_t v) {
	return v;
}

template<>
inline constexpr uint16_t byteorder<endian::big>::le<uint16_t>(uint16_t v) {
	return static_cast<uint16_t>((v << 8) | (v >> 8));
}

template<>
inline constexpr int16_t byteorder<endian::big>::le<int16_t>(int16_t v) {
	return static_cast<int16_t>(le<uint16_t>(static_cast<uint16_t>(v)));
}

template<>
constexpr uint32_t byteorder<endian::big>::le<uint32_t>(uint32_t v) {
	return
		(le<uint16_t>(static_cast<uint16_t>( v >> 16 ))       ) |
		(static_cast<uint32_t>(le<uint16_t>(static_cast<uint16_t>(v))) << 16 );
}

template<>
constexpr int32_t byteorder<endian::big>::le<int32_t>(int32_t v) {
	return static_cast<int32_t>(le<uint32_t>(static_cast<uint32_t>(v)));
}

template<>
constexpr uint64_t byteorder<endian::big>::le<uint64_t>(uint64_t v) {
	return
		(le<uint32_t>(static_cast<uint32_t>( v >> 32 ))       ) |
		(static_cast<uint64_t>(le<uint32_t>(static_cast<uint32_t>(v))) << 32 );
}

template<>
constexpr int64_t byteorder<endian::big>::le<int64_t>(int64_t v) {
	return static_cast<int64_t>(le<uint64_t>(static_cast<uint64_t>(v)));
}


}
