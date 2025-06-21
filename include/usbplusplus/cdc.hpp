/* Copyright (C) 2025 Michiel van Leeuwen <michiel@embeddedacoustics.com>
 *
 * cdc.hpp - USB++ classes related to CDC
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
#include "utils.hpp"

namespace usbplusplus {
namespace cdc {

enum class CdcInterfaceSubclassCode_t : uint8_t {
	Reseved							= 0x00,
	DirectLineControlModel			= 0x01,
	AbstractControlModel			= 0x02,
	TelephoneControlModel			= 0x03,
	MultiChannelControlModel		= 0x04,
	CAPIControlModel				= 0x05,
	EthernetNetworkingControlModel	= 0x06,
	ATMNetworkingControlModel		= 0x07,
	WirelessHandsetControlModel		= 0x08,
	DeviceManagement				= 0x09,
	MobileDirectLineModel			= 0x0A,
	OBEX							= 0x0B,
	EthernetEmulationModel			= 0x0C,
	NetworkControlModel				= 0x0D,
};

enum class CdcInterfaceProtocol_t : uint8_t {
	None 							= 0x00,
	ITU_T_V250						= 0x01,
	PCCA101							= 0x02,
	PCCA101_AnnexO					= 0x03,
	GSM7_07							= 0x04,
	_3GPP_27_07						= 0x05,
	C_S0017_0						= 0x06,
	USB_EEM							= 0x07,
	External						= 0xFE,
};


/* Table 12: CDC Class-specific Descriptor Types */
enum class CdcDescriptorType_t : uint8_t {
	CS_INTERFACE				= 0x24,
	CS_ENDPOINT					= 0x25
};

/* Table 13: bDescriptor SubType in CDC functional descriptors */
enum class CdcDescriptorSubType_t : uint8_t {
	CdcHeader					= 0x00,
	CallManagement				= 0x01,
	AbstractControlManangment	= 0x02,
	DirectLineManangment		= 0x03,
	TelephoneRinger				= 0x04,
	TelephoneCallsAndLineStateReportingCapabilities	= 0x05,
	Union						= 0x06,
	CountrySelection			= 0x07,
	TelephoneOperationalModes	= 0x08,
	UsbTerminal					= 0x09,
	NetworkChannelTerminal		= 0x0A,
	ProtocolUnit				= 0x0B,
	ExtensionUnit				= 0x0C,
	MultiChannelManagement		= 0x0D,
	CAPIControlManagement		= 0x0E,
	EthernetNetworking			= 0x0F,
	ATMNetworking				= 0x10,
	WirelessHandsetControlModel	= 0x11,
	MobileDirectLineModel		= 0x12,
	MDLMDetail					= 0x13,
	DeviceManagement			= 0x14,
	OBEX						= 0x15,
	CommandSet					= 0x16,
	CommandSetDetail			= 0x17,
	TelephoneControlModel		= 0x18,
	OBEXServiceIdentified		= 0x19,
	NCM							= 0x1A,
};

enum class CallManagementCapabilities_t : uint8_t {
	/**
	 * 0 - Device sends/receives call management information only over the Communications Class 
	 * interface.
	 * 1 - Device can send/receive call management information over a Data Class interface.
	 */
	UseDataClassInterface		= D(1),
	/**
	 * 0 - Device does not handle call management itself.
	 * 1 - Device handles call management itself.
	 */
	HandleCallManagement		= D(0),

	__mask						= D(1) | D(0),
};

enum class AbstractControlManagementCapabilities_t : uint8_t {
	/**
	 * 1 - Device supports the notification Network_Connection.
	 */
	NetworkConnection			= D(3),
	/**
	 * 1 - Device supports the request Send_Break
	 */
	SendBreak					= D(2),
	/**
	 * 1 - Device supports the request combination of Set_Line_Coding, Set_Control_Line_State,
	 * Get_Line_Coding, and the notification Serial_State.
	 */
	StateAndCoding				= D(1),
	/**
	 * 1 - Device supports the request combination of Set_Comm_Feature, Clear_Comm_Feature, and
	 * Get_Comm_Feature.
	 */
	CommFeatures				= D(0),

	__mask						= D(3) | D(2) | D(1) | D(0),
};

/** ECM Table 4. Ethernet Networking Functional Descriptor, bmEthernetStatistics */
enum class EthernetStatistics_t : uint32_t {
	XMIT_OK						= D(0),
	RVC_OK						= D(1),
	XMIT_ERROR					= D(2),
	RCV_ERROR					= D(3),
	RCV_NO_BUFFER				= D(4),
	DIRECTED_BYTES_XMIT			= D(5),
	DIRECTED_FRAMES_XMIT		= D(6),
	MULTICAST_BYTES_XMIT		= D(7),
	MULTICAST_FRAMES_XMIT		= D(8),
	BROADCAST_BYTES_XMIT		= D(9),
	BROADCAST_FRAMES_XMIT		= D(10),
	DIRECTED_BYTES_RCV			= D(11),
	DIRECTED_FRAMES_RCV			= D(12),
	MULTICAST_BYTES_RCV			= D(13),
	MULTICAST_FRAMES_RCV		= D(14),
	BROADCAST_BYTES_RCV			= D(15),
	BROADCAST_FRAMES_RCV		= D(16),
	RCV_CRC_ERROR				= D(17),
	TRANSMIT_QUEUE_LENGTH		= D(18),
	RCV_ERROR_ALIGNMENT			= D(19),
	XMIT_ONE_COLLISION			= D(20),
	XMIT_MORE_COLLISIONS		= D(21),
	XMIT_DEFERRED				= D(22),
	XMIT_MAX_COLLISIONS			= D(23),
	RCV_OVERRUN					= D(24),
	XMIT_UNDERRUN				= D(25),
	XMIT_HEARTBEAT_FAILURE		= D(26),
	XMIT_TIMES_CRS_LOST			= D(27),
	XMIT_LATE_COLLISIONS		= D(28),
};

/*****************************************************************************/
/*   AND, OR operators														 */
/*****************************************************************************/
} // namespace cdc

template<> inline constexpr bool enable_or<cdc::CallManagementCapabilities_t> = true;
template<> inline constexpr bool enable_and<cdc::CallManagementCapabilities_t> = true;

template<> inline constexpr bool enable_or<cdc::AbstractControlManagementCapabilities_t> = true;
template<> inline constexpr bool enable_and<cdc::AbstractControlManagementCapabilities_t> = true;

template<> inline constexpr bool enable_or<cdc::EthernetStatistics_t> = true;
template<> inline constexpr bool enable_and<cdc::EthernetStatistics_t> = true;

namespace cdc {
/*****************************************************************************/
/*   Field types															 */
/*****************************************************************************/

using CdcInterfaceClassCode = detail::constant<ClassCode_t, ClassCode_t::CDC>;
using CdcDataInterfaceClassCode = detail::constant<ClassCode_t, ClassCode_t::CDC_Data>;
using CdcInterfaceSubclassCode = CdcInterfaceSubclassCode_t;
using CdcInterfaceProtocol = CdcInterfaceProtocol_t;
using None = Empty;


template<typename T>
struct __attribute__((__packed__))
CdcDescriptorType : detail::typed<CdcDescriptorType_t> {
	constexpr CdcDescriptorType() :
		detail::typed<CdcDescriptorType_t>(T::descriptortype()) {}
};

template<typename T>
struct __attribute__((__packed__))
CdcDescriptorSubType : detail::typed<CdcDescriptorSubType_t> {
	constexpr CdcDescriptorSubType() :
		detail::typed<CdcDescriptorSubType_t>(T::descriptorsubtype()) {}
};

struct __attribute__((__packed__))
CdcHeaderFunctionalDescriptor {
	using self = CdcHeaderFunctionalDescriptor;
	static constexpr CdcDescriptorType_t descriptortype() {
		return CdcDescriptorType_t::CS_INTERFACE;
	}
	static constexpr CdcDescriptorSubType_t descriptorsubtype() {
		return CdcDescriptorSubType_t::CdcHeader;
	}
	static constexpr uint8_t length() {
		return sizeof(CdcHeaderFunctionalDescriptor);
	}

	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	CdcDescriptorType<self>		bDescriptorType;
	CdcDescriptorSubType<self>	bDescriptorSubType;
	BCD							bcdCDC;
};

template<unsigned N>
struct __attribute__((__packed__))
CdcUnionFunctionalDescriptor {
	using self = CdcUnionFunctionalDescriptor<N>;
	static constexpr CdcDescriptorType_t descriptortype() {
		return CdcDescriptorType_t::CS_INTERFACE;
	}
	static constexpr CdcDescriptorSubType_t descriptorsubtype() {
		return CdcDescriptorSubType_t::Union;
	}
	static constexpr uint8_t length() {
		return sizeof(self);
	}

	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	CdcDescriptorType<self>		bDescriptorType;
	CdcDescriptorSubType<self>	bDescriptorSubType;
	InterfaceNumber				bControlInterface;
	InterfaceNumber				bSubordinateInterface[N];
};

struct __attribute__((__packed__))
CallManagementFunctionalDescriptor {
	using self = CallManagementFunctionalDescriptor;
	static constexpr CdcDescriptorType_t descriptortype() {
		return CdcDescriptorType_t::CS_INTERFACE;
	}
	static constexpr CdcDescriptorSubType_t descriptorsubtype() {
		return CdcDescriptorSubType_t::CallManagement;
	}
	static constexpr uint8_t length() {
		return sizeof(self);
	}

	/* ------------------------------------------------*/
	Length<self>				bLength;
	CdcDescriptorType<self>		bDescriptorType;
	CdcDescriptorSubType<self>	bDescriptorSubType;
	CallManagementCapabilities_t	bmCapabilities;
	InterfaceNumber				bDataInterface;
};

struct __attribute__((__packed__))
AbstractControlManagementFunctionalDescriptor {
	using self = AbstractControlManagementFunctionalDescriptor;
	static constexpr CdcDescriptorType_t descriptortype() {
		return CdcDescriptorType_t::CS_INTERFACE;
	}
	static constexpr CdcDescriptorSubType_t descriptorsubtype() {
		return CdcDescriptorSubType_t::AbstractControlManangment;
	}
	static constexpr uint8_t length() {
		return sizeof(self);
	}

	/* ------------------------------------------------*/
	Length<self>				bLength;
	CdcDescriptorType<self>		bDescriptorType;
	CdcDescriptorSubType<self>	bDescriptorSubType;
	AbstractControlManagementCapabilities_t	bmCapabilities;
};

struct __attribute__((__packed__))
EthernetNetworkingFunctionDescriptor {
	using self = EthernetNetworkingFunctionDescriptor;
	static constexpr CdcDescriptorType_t descriptortype() {
		return CdcDescriptorType_t::CS_INTERFACE;
	}
	static constexpr CdcDescriptorSubType_t descriptorsubtype() {
		return CdcDescriptorSubType_t::EthernetNetworking;
	}
	static constexpr uint8_t length() {
		return sizeof(self);
	}

	/* ------------------------------------------------*/
	Length<self>				bLength;
	CdcDescriptorType<self>		bDescriptorType;
	CdcDescriptorSubType<self>	bDescriptorSubType;
	Index						iMACAddress;
	EthernetStatistics_t		bmEthernetStatistics;
	Number<2>					wMaxSegmentSize;
	Number<2>					wNumberMCFilters;
	Number<1>					bNumberPowerFilters;
};


template<typename FunctionalDescriptorCollection = None, typename NotificationEndpoints = None>
struct __attribute__((__packed__))
CdcControl {
	using self = CdcControl<FunctionalDescriptorCollection, NotificationEndpoints>;
	using Endpoints = typename NotificationEndpoints::type;
	using FunctionalDescriptors = typename FunctionalDescriptorCollection::type;

	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::INTERFACE;
	}
	static constexpr FixedNumber<self> numendpoints() {
		return FixedNumber<self>(NotificationEndpoints::count);
	}
	static constexpr uint8_t length() {
		return sizeof(CdcControl<None, None>) - sizeof(CdcHeaderFunctionalDescriptor);
	}
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>					bLength;
	DescriptorType<self>			bDescriptorType;
	InterfaceNumber					bInterfaceNumber;
	AlternateSetting				bAlternateSetting;
	NumEndpoints<self>				bNumEndpoints;
	CdcInterfaceClassCode			bInterfaceClass;
	CdcInterfaceSubclassCode		bInterfaceSubClass;
	CdcInterfaceProtocol			bInterfaceProtocol;
	Index							iInterface;
	CdcHeaderFunctionalDescriptor	header;
	FunctionalDescriptors			functional_descriptors;
	Endpoints						endpoints;
};

using ZeroBandwidth = usb2::Interface<Empty>;

using CdcEcmControl = CdcControl<
	List<CdcUnionFunctionalDescriptor<1>, EthernetNetworkingFunctionDescriptor>,
	List<usb2::Endpoint>
>;

using CdcAcmControl = CdcControl<
	List<
		CallManagementFunctionalDescriptor,
		AbstractControlManagementFunctionalDescriptor,
		CdcUnionFunctionalDescriptor<1>
	>,
	List<usb2::Endpoint>
>;

}
}
