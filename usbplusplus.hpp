/* Copyright (C) 2018 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * USB++ - is a C++14 template library for handy descriptor definition
 *
 * usbplusplus.hpp - main header file
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


/**
 * Rationales
 * 1. Design decisions
 * 1.1. Every field is declared with it specific type. This approach
 *      allows to handle:
 *         a) bit-level defined fields
 *         b) endiannes of multi-byte fields
 *         c) verbosity (NumConfigurations(1) is more verbose than just 1)
 * 1.2. All members that, for a given descriptor instance, have only one valid
 *      value (such as bLength, bDescriptorType, bNumInterfaces, etc)
 *      are encapsulated in a class-depended type with default constructor using
 *      the right value
 * 2. Naming convention
 * 2.1. Major entities (classes templates named by chapter title from the spec,
 *      e.g. Device, Qualifier
 * 2.2. Class members are named as in the table
 * 2.3. Data types for the class members are named by removing
 *      leading lower-case designator from the field name
 * 2.4. Enums are named with suffix _t to avoid collision with data types
 * 2.5. Helper methods for computing constant values are named by lower casing
 *      the field name
 * 3. USB1/USB2 revision related
 * 3.1. When there is a difference in USB1 and USB2 entities,
 *    USB2 placed in namespace usb2
 * 3.2. Enums (constants in terms of the spec) that were extended in USB2
 *    placed in outer namespace (usbplusplus) and contain all USB2 and USB1
 *    items
 * 3.3. Common field types are placed in outer namespace (usbplusplus)
 * 3.4. Revision-specific field types are placed in revision's namespace
 * 4. Literal operators are used for BCD and mA data types
 *
 * Based on USB 2.0 specification.
 * http://www.usb.org/developers/docs/usb20_docs/usb_20_020718.zip
 * References are to usb_20.pdf, unless  another source is explicitly stated.
 */

#include <cstdint>
#include "usblangids.hpp"
#include "byteorder.hpp"
#include "ustring.hpp"

namespace usbplusplus {

/*****************************************************************************/
/*   Constants, common to, or shared between USB1 and USB2					 */
/*****************************************************************************/

/** http://www.usb.org/developers/defined_class	(June 15, 2016)				*/
enum class ClassCode_t : uint8_t { //TODO _t
	Defined_in_the_Interface_Descriptors = 0x00,
	Audio 								= 0x01,
	Communications_and_CDC_Control 		= 0x02,
	CDC = Communications_and_CDC_Control,
	Human_Interface_Device 				= 0x03,
	HID = Human_Interface_Device,
	Physical							= 0x05,
	Image								= 0x06,
	Printer								= 0x07,
	Mass_Storage						= 0x08,
	Hub									= 0x09,
	CDC_Data							= 0x0A,
	Smart_Card							= 0x0B,
	Content_Security					= 0x0D,
	Video								= 0x0E,
	Personal_Healthcare					= 0x0F,
	Audio_Video_Devices					= 0x10,
	Billboard_Device_Class				= 0x11,
	USB_Type_C_Bridge_Class				= 0x12,
	Diagnostic_Device					= 0xDC,
	Wireless_Controller					= 0xE0,
	Miscellaneous						= 0xEF,
	Application_Specific				= 0xFE,
	Vendor_Specific						= 0xFF
};

/** Bit digit
 *  The specification uses D to define bits,
 *  so it is introduced here to make the implementation
 *  following the specification 											*/
constexpr unsigned D(unsigned N, unsigned val = 1) { return val << N; }

/* Table 9-2. Format of Setup Data											*/
enum class DataTransferDirection_t : uint8_t {
	Host_to_device	= 0,
	Device_to_Host	= D(7),
	__mask 			= D(7)
};

/* Table 9-2. Format of Setup Data											*/
enum class RequestType_t : uint8_t {
	Standard		= D(5, 0),
	Class			= D(5, 1),
	Vendor			= D(5, 2),
	__mask			= D(6) | D(5)
};


/* Table 9-2. Format of Setup Data											*/
enum class Recipient_t : uint8_t {
	Device			= D(0, 0),
	Interface		= D(0, 1),
	Endpoint		= D(0, 2),
	Other			= D(0, 3),
	__mask			= D(3) | D(2) | D(1) | D(0)
};

/* Table 9-4. Standard Request Codes										*/
enum class RequestCode_t : uint8_t {
	GET_STATUS 			=  0,
	CLEAR_FEATURE 		=  1,
	/* Reserved for future use 2											*/
	SET_FEATURE 		=  3,
	/* Reserved for future use 4											*/
	SET_ADDRESS 		=  5,
	GET_DESCRIPTOR 		=  6,
	SET_DESCRIPTOR 		=  7,
	GET_CONFIGURATION 	=  8,
	SET_CONFIGURATION 	=  9,
	GET_INTERFACE 		= 10,
	SET_INTERFACE 		= 11,
	SYNCH_FRAME 		= 12
};

/* Table 9-5. Descriptor Types												 */
enum class DescriptorType_t : uint8_t {
	DEVICE 			 = 1,
	CONFIGURATION    = 2,
	STRING           = 3,
	INTERFACE        = 4,
	ENDPOINT         = 5,
	DEVICE_QUALIFIER = 6,
	OTHER_SPEED      = 7,
	INTERFACE_POWER  = 8,
	OTG              = 9,
	/* USB ECN : Interface Association Descriptor
	 * InterfaceAssociationDescriptor_ecn.pdf								*/
	INTERFACE_ASSOCIATION = 11,
	/* http://www.usb.org/wusb/docs/WirelessUSBSpecification_r10.pdf		*/
	/* Table 7-25. Wireless USB Standard Extension Descriptor Types			*/
	SECURITY 		 = 12,
	KEY 			 = 13,
	ENCRYPTION_TYPE  = 14,
	BOS 			 = 15,
	DEVICE_CAPABILITY= 16,
	WIRELESS_ENDPOINT_COMPANION = 17,

};

/* Table 9-6. Standard Feature Selectors									*/
enum class FeatureSelector_t : uint8_t {
	DEVICE_REMOTE_WAKEUP = 1,
	ENDPOINT_HALT 		 = 0,
	TEST_MODE			 = 2
};

/* Table 9-7. Test Mode Selectors											*/
enum class TestModeSelector_t : uint8_t {
	Reserved		= 0x00,
	Test_J			= 0x01,
	Test_K			= 0x02,
	Test_SE0_NAK	= 0x03,
	Test_Packet		= 0x04,
	Test_Force_Enable = 0x05
/* 06H-3FH Reserved for standard test selectors
   3FH-BFH Reserved
   C0H-FFH Reserved for vendor-specific test modes.
 */
};

/* Table 9-8. Standard Device Descriptor 									*/
/** Maximum packet size for endpoint zero (only 8, 16, 32, or 64 are valid) */
enum class MaxPacketSize0_t : uint8_t {
	_8  = 8,
	_16 = 16,
	_32 = 32,
	_64 = 64
};

/** Table 9-10. Standard Configuration Descriptor							*/
enum class ConfigurationCharacteristics_t : uint8_t { //TODO _t
	None,
	Reserved      = D(7),
	Self_powered  = D(6),
	Remote_Wakeup = D(5)
};

/* Table 9-13. Standard Endpoint Descriptor									*/
enum class EndpointDirection_t : uint8_t {
	OUT, IN
};

/** Table 9-13. Standard Endpoint Descriptor 								*/
enum class TransferType_t : uint8_t {
	Control		= 0b00,
	Isochronous	= 0b01,
	Bulk		= 0b10,
	Interrupt	= 0b11,
	__mask		= 0b11
};

namespace detail {

template<int Size, typename Signed = unsigned>
class __attribute__((__packed__))
field {
protected:
	using type = typename intN_t<Size,Signed>::type;
	type value;
	constexpr const type* ptr() const { return &value; }
public:
	constexpr type get() const { return byteorder<>::ne(value); }
	constexpr field(type v) : value(byteorder<>::le(v)) {}
};

template<typename S>
class __attribute__((__packed__))
typed {
protected:
	using type = S;
	type value;
public:
	constexpr typed(type v) : value(byteorder<>::le(v)) {}
	constexpr type get() const { return byteorder<>::ne(value); }
};

template<typename S, S V>
class __attribute__((__packed__))
constant : protected typed<S> {
public:
	constexpr constant() : typed<S>(V) {}
	using typed<S>::get;
};


/** Extract first argument from the parameter pack							 */
template<typename ... List>
struct first;

template<typename First>
struct first<First> {
	using type = First;
};


template<typename First, typename ... List>
struct first<First, List...> {
	using type = First;
};

/** one-based list indexer  												*/
template<typename T, T ... List>
struct index;

template<typename T, T First>
struct index<T, First> {
	static constexpr unsigned of(T val) {
		return val == First ? 1 : 0;
	}
};

template<typename T, T First, T ... List>
struct index<T, First, List...> {
	static constexpr unsigned of(T val) {
		return val == First ? 1 : incifnz(index<T, List...>::of(val));
	}
};

using string_getter = const uint8_t* (*)();
using mstring_getter = const uint8_t* (*)(uint8_t, LanguageIdentifier);

template<typename T>
inline constexpr T operator_or(T a, T b) {
	using type = typename intN_t<sizeof(T),unsigned>::type;
	return static_cast<T>(static_cast<type>(a) | static_cast<type>(b));
}

template<typename T>
inline constexpr T operator_and(T a, T b) {
	using type = typename intN_t<sizeof(T),unsigned>::type;
	return static_cast<T>(static_cast<type>(a) & static_cast<type>(b));
}

template<typename T, typename O>
inline constexpr typename intN_t<sizeof(T),unsigned>::type
_or(T a, O b) {
	using type = typename intN_t<sizeof(T),unsigned>::type;
	return static_cast<type>(a) | static_cast<type>(b);
}

template<typename T, typename O>
inline constexpr typename intN_t<sizeof(T),unsigned>::type
_and(T a, O b) {
	using type = typename intN_t<sizeof(T),unsigned>::type;
	return static_cast<type>(a) & static_cast<type>(b);
}


}

/*****************************************************************************/
/*   AND, OR operators														 */
/*****************************************************************************/

inline constexpr DataTransferDirection_t operator&(DataTransferDirection_t a,
		DataTransferDirection_t b) {
	return detail::operator_and(a,b);
}

inline constexpr DataTransferDirection_t operator|(DataTransferDirection_t a,
		DataTransferDirection_t b) {
	return detail::operator_or(a,b);
}

inline constexpr RequestType_t operator&(RequestType_t a,	RequestType_t b) {
	return detail::operator_and(a,b);
}

inline constexpr RequestType_t operator|(RequestType_t a,	RequestType_t b) {
	return detail::operator_or(a,b);
}

inline constexpr Recipient_t operator&(Recipient_t a,	Recipient_t b) {
	return detail::operator_and(a,b);
}

inline constexpr Recipient_t operator|(Recipient_t a,	Recipient_t b) {
	return detail::operator_or(a,b);
}

inline constexpr TransferType_t operator&(TransferType_t a, TransferType_t b) {
	return detail::operator_and(a,b);
}

inline constexpr TransferType_t operator|(TransferType_t a, TransferType_t b) {
	return detail::operator_or(a,b);
}

/*****************************************************************************/
/*   Field types															 */
/*****************************************************************************/

class __attribute__((__packed__))
RequestType : protected detail::field<1> {
	constexpr RequestType(DataTransferDirection_t direction,
			RequestType_t requesttype, Recipient_t recipient) :
		field<1>(detail::_and(direction, DataTransferDirection_t::__mask) |
				detail::_and(requesttype, RequestType_t::__mask ) |
				detail::_and(recipient, Recipient_t::__mask)) {
	}
	constexpr DataTransferDirection_t dataTransferDirection() const {
		return static_cast<DataTransferDirection_t>(
				get() & static_cast<type>(DataTransferDirection_t::__mask));
	}
	constexpr RequestType_t requestType() const {
		return static_cast<RequestType_t>(
				get() & static_cast<type>(RequestType_t::__mask));
	}
	constexpr Recipient_t recipient() const {
		return static_cast<Recipient_t>(
				get() & static_cast<type>(Recipient_t::__mask));
	}
};

class __attribute__((__packed__))
BCD : protected detail::field<2> {
public:
	using detail::field<2>::get;
private:
	constexpr BCD(type v) : detail::field<2>(v) {}
	friend constexpr BCD operator "" _bcd(long double v);
};

constexpr BCD operator "" _bcd(long double v) {
	return BCD(
		(static_cast<unsigned>(v/10 ) % 10) << 12 |
		(static_cast<unsigned>(v    ) % 10) <<  8 |
		(static_cast<unsigned>(v*10 ) % 10) <<  4 |
		(static_cast<unsigned>(v*100) % 10));
}

class __attribute__((__packed__))
MaxPower : protected detail::field<1> {
public:
	using detail::field<1>::get;
private:
	constexpr MaxPower(type v) : field<1>(v) {}
	friend constexpr MaxPower operator "" _mA(unsigned long long v);
};

constexpr MaxPower operator "" _mA(unsigned long long v) {
	return MaxPower(v / 2); /* Maximum power expressed in 2 mA units */
}

struct __attribute__((__packed__))
Interval : detail::field<1> {
	using typename detail::field<1>::type;
	constexpr Interval(type v) : detail::field<1>(v) {}
};


template<int Size>
class __attribute__((__packed__))
Reserved : detail::field<Size> {
public:
	constexpr Reserved() : detail::field<Size>(0) {}
};

template<typename T>
struct __attribute__((__packed__))
Length : protected detail::field<1> {
	using detail::field<1>::get;
	using detail::field<1>::ptr;
	constexpr Length() : detail::field<1>(T::length()) {};
};

template<typename T>
struct __attribute__((__packed__))
DescriptorType : detail::typed<DescriptorType_t> {
	constexpr DescriptorType() :
		detail::typed<DescriptorType_t>(T::descriptortype()) {}
};


template<typename T>
struct __attribute__((__packed__))
TotalLength : protected detail::field<2> {
	using detail::field<2>::get;
	constexpr TotalLength() : detail::field<2>(T::totallength()) {};
};

/** FixedNumber - base class for a number, inferred from the descriptors */
template<typename T>
struct __attribute__((__packed__))
FixedNumber : protected detail::field<1> {
	using typename detail::field<1>::type;
	constexpr FixedNumber(type v) : detail::field<1>(v) {};
};

struct __attribute__((__packed__))
SubClassCode : detail::field<1> {
	using typename detail::field<1>::type;
	constexpr SubClassCode(type value) : detail::field<1>(value) {}
};

struct __attribute__((__packed__))
ProtocolCode : detail::field<1> {
	using typename detail::field<1>::type;
	constexpr ProtocolCode(type value) : detail::field<1>(value) {}
};

struct __attribute__((__packed__))
ID : detail::field<2> {
	using typename field<2>::type;
	constexpr ID(type value) : detail::field<2>(value) {}
};

struct __attribute__((__packed__))
Index : detail::field<1> {
	using typename detail::field<1>::type;
	constexpr Index(type value) : detail::field<1>(value) {}
	constexpr Index() : detail::field<1>(0) {}
};

struct __attribute__((__packed__))
NumConfigurations : detail::field<1> {
	using typename field<1>::type;
	constexpr NumConfigurations(type value) : detail::field<1>(value) {}
};

struct __attribute__((__packed__))
NumInterfaces : detail::field<1> {
	using typename field<1>::type;
	constexpr NumInterfaces(type value) : detail::field<1>(value) {}
};

template<typename T>
struct __attribute__((__packed__))
NumEndpoints : protected FixedNumber<T> {
	using typename FixedNumber<T>::type;
	using FixedNumber<T>::get;
	constexpr NumEndpoints() : FixedNumber<T>(T::numendpoints()) {}
};


/** Number - a number to be provided by the user */
template<uint8_t N = 1>
struct __attribute__((__packed__))
Number : protected detail::field<N> {
	using typename detail::field<N>::type;
	constexpr Number(type v) : detail::field<N>(v) {};
};

struct __attribute__((__packed__))
InterfaceNumber : Number<1> {
	constexpr InterfaceNumber(type value) : Number<1>(value) {}
};

struct __attribute__((__packed__))
AlternateSetting : Number<1> {
	constexpr AlternateSetting(type value) : Number<1>(value) {}
};

struct __attribute__((__packed__))
ConfigurationValue : Number<1> {
	constexpr ConfigurationValue(type value) : Number<1>(value) {}
};

struct __attribute__((__packed__))
EndpointAddress : detail::field<1> {
	using typename detail::field<1>::type;
	constexpr EndpointAddress(type number, EndpointDirection_t dir) :
		field<1>((number & 0x3) | (static_cast<type>(dir) << 7)) {}
};

struct __attribute__((__packed__))
MaxPacketSize : detail::field<2> {
	using typename detail::field<2>::type;
	constexpr MaxPacketSize(type size) :
		field<2>(size) {}
};

/*****************************************************************************/
/*  Collections																 */
/*****************************************************************************/

struct __attribute__((__packed__))
Empty {
	using type = char[0];
	static constexpr unsigned count = 0;
};

template<class Item, unsigned Count>
struct __attribute__((__packed__))
Array {
	using type = Item[Count];
	static constexpr unsigned count = Count;
};

template<class ... Item>
struct List;

template<class Item0>
struct __attribute__((__packed__))
List<Item0> {
	static constexpr unsigned count = 1;
	struct __attribute__((__packed__)) type {
		Item0 item0;
	};
};
template<class Item0, class Item1>
struct __attribute__((__packed__))
List<
		Item0, Item1> {
	static constexpr unsigned count = 2;
	struct __attribute__((__packed__)) type {
		Item0 item0;
		Item1 item1;
	};
};
template<class Item0, class Item1, class Item2>
struct __attribute__((__packed__))
List<
		Item0, Item1, Item2> {
	static constexpr unsigned count = 3;
	struct __attribute__((__packed__)) type {
		Item0 item0;
		Item1 item1;
		Item2 item2;
	};
};
template<class Item0, class Item1, class Item2, class Item3>
struct __attribute__((__packed__))
List<
		Item0, Item1, Item2, Item3> {
	static constexpr unsigned count = 4;
	struct __attribute__((__packed__)) type {
		Item0 item0;
		Item1 item1;
		Item2 item2;
		Item3 item3;
	};
};
template<class Item0, class Item1, class Item2, class Item3, class Item4>
struct __attribute__((__packed__))
List<
		Item0, Item1, Item2, Item3, Item4> {
	static constexpr unsigned count = 5;
	struct __attribute__((__packed__)) type {
		Item0 item0;
		Item1 item1;
		Item2 item2;
		Item3 item3;
		Item4 item4;
	};
};
template<class Item0, class Item1, class Item2, class Item3, class Item4,
		class Item5>
struct __attribute__((__packed__))
List<Item0, Item1,
		Item2, Item3, Item4, Item5> {
	static constexpr unsigned count = 6;
	struct __attribute__((__packed__)) type {
		Item0 item0;
		Item1 item1;
		Item2 item2;
		Item3 item3;
		Item4 item4;
		Item5 item5;
	};
};
template<class Item0, class Item1, class Item2, class Item3, class Item4,
		class Item5, class Item6>
struct __attribute__((__packed__))
List<Item0,
		Item1, Item2, Item3, Item4, Item5, Item6> {
	static constexpr unsigned count = 7;
	struct __attribute__((__packed__)) type {
		Item0 item0;
		Item1 item1;
		Item2 item2;
		Item3 item3;
		Item4 item4;
		Item5 item5;
		Item6 item6;
	};
};
template<class Item0, class Item1, class Item2, class Item3, class Item4,
		class Item5, class Item6, class Item7>
struct __attribute__((__packed__))
List<
		Item0, Item1, Item2, Item3, Item4, Item5, Item6, Item7> {
	static constexpr unsigned count = 8;
	struct __attribute__((__packed__)) type {
		Item0 item0;
		Item1 item1;
		Item2 item2;
		Item3 item3;
		Item4 item4;
		Item5 item5;
		Item6 item6;
		Item7 item7;
	};
};

template<class Item0, class Item1, class Item2, class Item3, class Item4,
		class Item5, class Item6, class Item7, class Item8>
struct __attribute__((__packed__))
List<Item0, Item1, Item2, Item3, Item4, Item5, Item6, Item7, Item8> {
	static constexpr unsigned count = 8;
	struct __attribute__((__packed__)) type {
		Item0 item0;
		Item1 item1;
		Item2 item2;
		Item3 item3;
		Item4 item4;
		Item5 item5;
		Item6 item6;
		Item7 item7;
		Item8 item8;
	};
};

template<class Item0, class Item1, class Item2, class Item3, class Item4,
		class Item5, class Item6, class Item7, class Item8, class Item9>
struct __attribute__((__packed__))
List<Item0, Item1, Item2, Item3, Item4, Item5, Item6, Item7, Item8, Item9> {
	static constexpr unsigned count = 8;
	struct __attribute__((__packed__)) type {
		Item0 item0;
		Item1 item1;
		Item2 item2;
		Item3 item3;
		Item4 item4;
		Item5 item5;
		Item6 item6;
		Item7 item7;
		Item8 item8;
		Item9 item9;
	};
};

template<class Item0, class Item1, class Item2, class Item3, class Item4,
		class Item5, class Item6, class Item7, class Item8, class Item9,
		class Item10>
struct __attribute__((__packed__))
List<Item0, Item1, Item2, Item3, Item4, Item5, Item6, Item7, Item8, Item9,
	Item10> {
	static constexpr unsigned count = 8;
	struct __attribute__((__packed__)) type {
		Item0 item0;
		Item1 item1;
		Item2 item2;
		Item3 item3;
		Item4 item4;
		Item5 item5;
		Item6 item6;
		Item7 item7;
		Item8 item8;
		Item9 item9;
		Item10 item10;
	};
};

/*****************************************************************************/
/*  USB1 entities 							 								 */
/*****************************************************************************/

namespace usb1 {
using DeviceClass 		= ClassCode_t;
using DeviceSubClass 	= SubClassCode;
using DeviceProtocol 	= ProtocolCode;
using InterfaceClass	= ClassCode_t;
using InterfaceSubClass	= uint8_t;
using InterfaceProtocol = ProtocolCode;
using MaxPacketSize0	= MaxPacketSize0_t;
using IDVendor			= ID;
using IDProduct			= ID;
using Manufacturer 		= Index;
using Product 			= Index;
using SerialNumber 		= Index;
using RequestCode		= RequestCode_t;
using Value				= detail::field<2>;
using DataLength		= detail::field<2>;

/*****************************************************************************/
/*  Table 9-2. Format of Setup Data											 */
/** Setup Packet															 */
struct __attribute__((__packed__))
SetupPacket {
	RequestType 				bmRequestType;
	RequestCode 				bRequest;
	Value						bValue;
	Index						wIndex;
	DataLength					wLength;
};
/*****************************************************************************/
/*  Table 9-8. Standard Device Descriptor									 */
/** Standard Device Descriptor												 */
struct __attribute__((__packed__))
Device {
	using self = Device;
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::DEVICE;
	}
	static constexpr uint8_t length() {	return sizeof(self); }
	const uint8_t* ptr() const { return bLength.ptr(); }
	/* ------------------------------------------------*/
	Length<self> 				bLength;
	DescriptorType<self> 		bDescriptorType;
	BCD							bcdUsb;
	DeviceClass					bDeviceClass;
	DeviceSubClass				bDeviceSubClass;
	DeviceProtocol				bDeviceProtocol;
	MaxPacketSize0				bMaxPacketSize0;
	IDVendor					idVendor;
	IDProduct					idProduct;
	BCD							bcdDevice;
	Index						iManufacturer;
	Index						iProduct;
	Index						iSerialNumber;
	NumConfigurations			bNumConfigurations;
};

/*****************************************************************************/
/*  Table 9-10. Standard Configuration Descriptor							 */
/** Standard Configuration Descriptor										 */
template<class InterfaceCollection>
struct __attribute__((__packed__))
Configuration {
	using self = Configuration<InterfaceCollection>;
	using Interfaces = typename InterfaceCollection::type;
	using Collection = InterfaceCollection;
	using Characteristics = ConfigurationCharacteristics_t;
	struct __attribute__((__packed__))
	Attributes : private detail::field<1> {
		constexpr Attributes(Characteristics c1 = Characteristics::None,
				Characteristics c2 = Characteristics::None)
		  : field<1>(static_cast<type>(c1) | static_cast<type>(c2) |
		             static_cast<type>(ConfigurationCharacteristics_t::Reserved)) {}
	};
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::CONFIGURATION;
	}
	static constexpr uint16_t totallength() { return sizeof(self); }
	static constexpr uint8_t length() {	return sizeof(Configuration<Empty>); }
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self> 				bLength;
	DescriptorType<self> 		bDescriptorType;
	TotalLength<self>			wTotalLength;
	NumInterfaces				bNumInterfaces;
	ConfigurationValue			bConfigurationValue;
	Index						iConfiguration;
	Attributes					bmAttributes;
	MaxPower					bMaxPower;
	Interfaces					interfaces;
};

/*****************************************************************************/
/*  Table 9-12. Standard Interface Descriptor								 */
/** Standard Interface Descriptor								 			 */
template<class EndpointCollection>
struct __attribute__((__packed__))
Interface {
	using self = Interface<EndpointCollection>;
	using Endpoints = typename EndpointCollection::type;
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::INTERFACE;
	}
	static constexpr FixedNumber<self> numendpoints() {
		return FixedNumber<self>(EndpointCollection::count);
	}
	static constexpr uint8_t length() {	return sizeof(Interface<Empty>); }
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self>		bDescriptorType;
	InterfaceNumber				bInterfaceNumber;
	AlternateSetting			bAlternateSetting;
	NumEndpoints<self>			bNumEndpoints;
	InterfaceClass				bInterfaceClass;
	InterfaceSubClass			bInterfaceSubClass;
	InterfaceProtocol			bInterfaceProtocol;
	Index						iInterface;
	/* the endpoint descriptors follow the interface descriptor 			 */
	Endpoints					endpoints;
};

/*****************************************************************************/
/** Alternate Settings - is just a list of interfaces			 			 */
template<typename ... Interfaces>
struct AlternateSettings : List<Interfaces...> {};

/*****************************************************************************/
/*  Table 9-13. Standard Endpoint Descriptor								 */
/** Standard Endpoint Descriptor (USB 1.0)									 */
struct __attribute__((__packed__))
Endpoint {
	using self = Endpoint;
	struct __attribute__((__packed__))
	Attributes : private detail::field<1> {
		constexpr Attributes(TransferType_t transferType)
		  : detail::field<1>(static_cast<type>(transferType) & 0b11) {}
	};
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::ENDPOINT;
	}
	static constexpr uint8_t length() { return sizeof(self); }
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self>		bDescriptorType;
	EndpointAddress				bEndpointAddress;
	Attributes					bmAttributes;
	MaxPacketSize				wMaxPacketSize;
	Interval					bInterval;
};

/*****************************************************************************/
/*  Table 9-15. String Descriptor Zero,
 *  Specifying Languages Supported by the Device 							 */
/** List of Supported Languages												 */
template<unsigned N>
struct __attribute__((__packed__))
Languages {
	using self = Languages;
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::STRING;
	}
	static constexpr uint8_t length() { return sizeof(self); }
	constexpr const uint8_t * ptr() const {	return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self>		bDescriptorType;
	LanguageIdentifier  		wLANGID[N];
};

/*****************************************************************************/
/*  Table 9-16. UNICODE String Descriptor									 */
/** String Descriptor												 		 */
template<ustring string>
struct String {
	using self = String<string>;
	static constexpr unsigned len = usbplusplus::length(string);
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::STRING;
	}
	static constexpr uint8_t length() { return sizeof(self); }
	constexpr const uint8_t * ptr() const {	return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self>		bDescriptorType;
	utf16le<len> 				bString {string};
};

}

/*****************************************************************************/
/*  USB2 entities 							 								 */
/*****************************************************************************/
namespace usb2 {
using usb1::DeviceClass;
using usb1::DeviceSubClass;
using usb1::DeviceProtocol;
using usb1::InterfaceClass;
using usb1::InterfaceSubClass;
using usb1::InterfaceProtocol;
using usb1::IDVendor;
using usb1::IDProduct;
using usb1::Manufacturer;
using usb1::Product;
using usb1::SerialNumber;
using usb1::Device;
using usb1::Interface;
using usb1::AlternateSettings;
using usb1::Configuration;
using usb1::Languages;
using usb1::String;

using FunctionClass 	= ClassCode_t;
using FunctionSubClass	= SubClassCode;
using FunctionProtocol	= ProtocolCode;

/*****************************************************************************/
/** Table 9-13. Standard Endpoint Descriptor, bmAttributes					 */
enum class SynchronizationType_t : uint8_t {
	No_Synchronization	= D(2,0b00),
	Asynchronous		= D(2,0b01),
	Adaptive			= D(2,0b10),
	Synchronous			= D(2,0b11),
	__mask				= D(3) | D(2)
};

inline constexpr uint8_t operator&(SynchronizationType_t a,
		SynchronizationType_t b) {
	return static_cast<uint8_t>(a) & static_cast<uint8_t>(b);
}

/** Table 9-13. Standard Endpoint Descriptor, bmAttributes					 */
enum class UsageType_t : uint8_t {
	Data_endpoint		= D(4, 0b00),
	Feedback_endpoint	= D(4, 0b01),
	Implicit_feedback_Data_endpoint = D(4, 0b10),
	//11 = Reserved
	__mask				= D(5) | D(4)
};

inline constexpr uint8_t operator&(UsageType_t a, UsageType_t b) {
	return static_cast<uint8_t>(a) & static_cast<uint8_t>(b);
}

/*****************************************************************************/
/* Table 9-9. Device_Qualifier Descriptor									 */
/** Device_Qualifier Descriptor												 */
struct __attribute__((__packed__))
Device_Qualifier {
	using self = Device_Qualifier;
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::DEVICE_QUALIFIER;
	}
	static constexpr uint8_t length() { return sizeof(self); }
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self> 		bDescriptorType;
	BCD							bcdUsb;
	DeviceClass					bDeviceClass;
	DeviceSubClass				bDeviceSubClass;
	DeviceProtocol				bDeviceProtocol;
	MaxPacketSize0_t			bMaxPacketSize0;
	NumConfigurations			bNumConfigurations;
	Reserved<1>					bReserved;
};

/*****************************************************************************/
/* Table 9-11. Other_Speed_Configuration Descriptor							 */
/** Other_Speed_Configuration												 */
template<class InterfaceCollection>
struct __attribute__((__packed__))
Other_Speed_Configuration {
	using self = Other_Speed_Configuration<InterfaceCollection>;
	using Interfaces = typename InterfaceCollection::type;
	using Characteristics = ConfigurationCharacteristics_t;
	struct __attribute__((__packed__))
	Attributes : private detail::field<1> {
		constexpr Attributes(Characteristics c1 = Characteristics::None,
				Characteristics c2 = Characteristics::None)
		  : field<1>(static_cast<type>(c1) | static_cast<type>(c2)) {}
	};
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::OTHER_SPEED;
	}
	static constexpr uint16_t totallength() { return sizeof(self); }
	static constexpr uint8_t numinterfaces() {
		return InterfaceCollection::count;
	}
	static constexpr uint8_t length() {
		return sizeof(Other_Speed_Configuration<Empty>);
	}
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self> 				bLength;
	DescriptorType<self> 		bDescriptorType;
	TotalLength<self>			wTotalLength;
	FixedNumber<self>			bNumInterfaces { numinterfaces() };
	ConfigurationValue			bConfigurationValue;
	Index						iConfiguration;
	Attributes					bmAttributes;
	MaxPower					bMaxPower;
	Interfaces					interfaces;
};

/*****************************************************************************/
/* Table 9-13. Standard Endpoint Descriptor									 */
/** Standard Endpoint Descriptor (USB 2.0)									 */
struct __attribute__((__packed__))
Endpoint {
	using self = Endpoint;
	struct __attribute__((__packed__))
	Attributes : private detail::field<1> {
		constexpr Attributes(TransferType_t transfer, SynchronizationType_t sync,
				UsageType_t usage = UsageType_t::Data_endpoint)
		  : detail::field<1>(
				 detail::_and(transfer, TransferType_t::__mask       )  |
				 detail::_and(sync,     SynchronizationType_t::__mask)  |
				 detail::_and(usage,    UsageType_t::__mask          )) {}
		constexpr Attributes(TransferType_t transferType)
		  : detail::field<1>(static_cast<type>(transferType) & 0b11) {}
	};
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::ENDPOINT;
	}
	static constexpr uint8_t length() {	return sizeof(self); }
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<Endpoint>			bLength;
	DescriptorType<Endpoint>	bDescriptorType;
	EndpointAddress				bEndpointAddress;
	Attributes					bmAttributes;
	MaxPacketSize				wMaxPacketSize;
	Interval					bInterval;
};

/*****************************************************************************/
/* Table 9-Z. Standard Interface Association Descriptor						 */
/* InterfaceAssociationDescriptor_ecn.pdf									 */
/** Standard Interface Association Descriptor								 */
struct __attribute__((__packed__))
InterfaceAssociation {
	using self = InterfaceAssociation;
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::INTERFACE_ASSOCIATION;
	}
	static constexpr uint8_t length() {	return sizeof(self); }
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self>		bDescriptorType;
	InterfaceNumber				bFirstInterface;
	Number<1>					bInterfaceCount;
	FunctionClass				bFunctionClass;
	FunctionSubClass			bFunctionSubClass;
	FunctionProtocol			bFunctionProtocol;
	Index 						iFunction;
};
}

/*****************************************************************************/
/*  Helper entities 							 							 */
/*****************************************************************************/

/** List of Language identifiers,
 *  Used to build string descriptor zero and for multilingual strings		 */
template<LanguageIdentifier ... List>
struct LanguageList {
	static constexpr unsigned count = sizeof...(List);
	using type = usb1::Languages<count>;
	static constexpr type list() {
		return { {}, {}, { List ... } };
	}
	static constexpr unsigned indexof(LanguageIdentifier lang) {
		return detail::index<LanguageIdentifier, List...>::of(lang);
	}
};

/**
 * usb1::String wrapper for accessing the descriptor via a getter
 */
template<ustring Source>
struct StringItem {
	static const uint8_t* get() {
		static const usb1::String<Source> source;
		return source.ptr();
	}
};

/**
 *  Monolingual dictionary of string descriptors
 */
template<LanguageIdentifier LangID, ustring ... List>
class Strings {
	static const uint8_t* getlangs() {
		static const typename LanguageList<LangID>::type languages = 
                    LanguageList<LangID>::list();
		return languages.ptr();
	}
public:
	/** Returns one-based index of str, for use in descriptor definitions 	 */
	static constexpr unsigned indexof(ustring str) {
		return list<List...>::indexof(str);
	}
	static constexpr Index::type count = sizeof...(List);
	static constexpr LanguageIdentifier lang = LangID;

	/** Returns pointer to a string descriptor, including String Descriptor Zero
	 * LanguageIdentifier is ignored										 */
	static const uint8_t* get(Index::type index, LanguageIdentifier l= LangID) {
		static constexpr detail::string_getter items[] {
			StringItem<List>::get ...
		};
		return index == 0u ? getlangs()
			 : index > count ? nullptr
			 : items[index-1]();
	}

};

/** Multilingual dictionary of string resources.
 *  Usage: MultiStrings<Strings<lang1, ustring ...> ...>
 *  All Strings are expected to have the same number of ustring
 *  in the same order across all Strings									*/
template<typename ... Lists>
class MultiStrings {
	using Langs = LanguageList<Lists::lang...>;
	static const uint8_t* getlangs() {
		static constexpr const typename Langs::type languages = Langs::list();
		return languages.ptr();
	}
public:
	static constexpr unsigned count = detail::first<Lists...>::type::count;
	static_assert(Langs::count == sizeof...(Lists),
			"Count of languages mismatches count of strings");

	/** Returns one-based index of str in the first list.
	 *  For use in descriptor definitions 	 								 */
	static constexpr unsigned indexof(ustring str) {
		return detail::first<Lists...>::type::indexof(str);
	}

	/** Returns pointer to a string descriptor, including String Descriptor Zero
	 * LanguageIdentifier is looked up in the Langs. If not found, */
	static const uint8_t* get(Index::type index, LanguageIdentifier lang) {
		static constexpr detail::mstring_getter items[] {
			Lists::get ...
		};
		if( index > count ) return nullptr;
		if( index == 0 ) return getlangs();  /* String Descriptor Zero		 */
		unsigned pos = Langs::indexof(lang); /* one-based index 			 */
		if( pos ) --pos; /* if not found, the first language is used		 */
		return items[pos](index, lang);
	}
};


}
