/* Copyright (C) 2018 Michiel van Leeuwen <michiel@embeddedacoustics.com>
 *
 * hid.hpp - USB++ classes related to HID
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

#include "usbplusplus.hpp"

namespace usbplusplus {
namespace hid {

enum class HidInterfaceSubclassCode_t : uint8_t {
	NONE 						= 0x00,
	BOOT						= 0x01,
};

enum class HidInterfaceProtocol_t : uint8_t {
	NONE 						= 0x00,
	KEYBOARD					= 0x01,
	MOUSE						= 0x01,
};

using HidInterfaceClassCode = detail::constant<ClassCode_t, ClassCode_t::HID>;
using HidInterfaceSubclassCode = HidInterfaceSubclassCode_t;
using HidInterfaceProtocol	= HidInterfaceProtocol_t;

// template<typename T>
// struct __attribute__((__packed__))
// HidInterfaceSubclassCode :
// 	public detail::constant<HidInterfaceSubclassCode_t, T::subclass()> {};



struct __attribute__((__packed__))
HidReportDescriptor {
	using self = HidReportDescriptor;
	static constexpr DescriptorType_t descriptortype() {
		return (DescriptorType_t)0x22;
	}

	/* ------------------------------------------------*/
	DescriptorType<self>		bDescriptorType;
	Number<2>           		wDescriptorLength;
};

template<uint8_t NReportDescriptors, typename EndpointCollection>
struct __attribute__((__packed__))
HidInterface {
	using self = HidInterface<NReportDescriptors, EndpointCollection>;
	using Endpoints = typename EndpointCollection::type;

	template<uint8_t N>
	struct __attribute__((__packed__))
	HidDescriptor {
		using self = HidDescriptor<N>;
		static constexpr uint8_t length() {
			return sizeof(self);
		}
		static constexpr DescriptorType_t descriptortype() {
			return (DescriptorType_t)0x21;
		}
		using NumDescriptors = detail::constant<uint8_t, N>;

		/* ------------------------------------------------*/
		Length<self>				bLength;
		DescriptorType<self>		bDescriptorType;
		BCD         				bcdHID;
		Number<1>					bCountryCode;
		NumDescriptors				bNumDescriptors;
		HidReportDescriptor			reportDescriptors[N];
	};

	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::INTERFACE;
	}
	static constexpr FixedNumber<self> numendpoints() {
		return FixedNumber<self>(EndpointCollection::count);
	}
	static constexpr uint8_t length() {
		return sizeof(HidInterface<NReportDescriptors, Empty>) - sizeof(HidDescriptor<NReportDescriptors>);
	}
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self>		bDescriptorType;
	InterfaceNumber				bInterfaceNumber;
	AlternateSetting			bAlternateSetting;
	NumEndpoints<self>			bNumEndpoints;
	HidInterfaceClassCode		bInterfaceClass;
	HidInterfaceSubclassCode	bInterfaceSubClass;
	HidInterfaceProtocol		bInterfaceProtocol;
	Index						iInterface;
	HidDescriptor<NReportDescriptors> hidDescriptor;
	Endpoints					endpoints;
};

}
}
