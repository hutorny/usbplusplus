/* Copyright (C) 2026 Daan van Oppenraaij <daan.vo@hotmail.com>
 *
 * uvc.hpp - USB++ classes related to the USB Video Class
 *
 * Based on the USB-IF Video Class v1.5 document set,
 * https://www.usb.org/document-library/video-class-v15-document-set
 *
 * UVC 1.5 Class specification.pdf
 *   Universal Serial Bus Device Class Definition for Video Devices, Revision 1.5
 * USB_Video_Payload_Frame_Based_1.5.pdf
 *   Universal Serial Bus Device Class Definition for Video Devices: Frame Based Payload, Revision 1.5
 *
 * These descriptors are unchanged since UVC 1.1, so they also serve bcdUVC 1.10 devices.
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
namespace uvc {

/* UVC 1.5 Table A-2: Video Interface Subclass Codes */
enum class VideoInterfaceSubclassCode_t : uint8_t {
	Undefined = 0x00,
	VideoControl = 0x01,
	VideoStreaming = 0x02,
	VideoInterfaceCollection = 0x03,
};

/* UVC 1.5 Table A-4: Video Class-Specific Descriptor Types */
enum class VideoDescriptorType_t : uint8_t {
	CS_INTERFACE = 0x24,
	CS_ENDPOINT = 0x25,
};

/* UVC 1.5 Table A-5: Video Class-Specific VC Interface Descriptor Subtypes, a subset */
enum class VCInterfaceDescriptorSubtype_t : uint8_t {
	Header = 0x01,
	InputTerminal = 0x02,
	OutputTerminal = 0x03,
	SelectorUnit = 0x04,
	ProcessingUnit = 0x05,
	ExtensionUnit = 0x06,
};

/* UVC 1.5 Table A-6: Video Class-Specific VS Interface Descriptor Subtypes, a subset */
enum class VSInterfaceDescriptorSubtype_t : uint8_t {
	InputHeader = 0x01,
	OutputHeader = 0x02,
	StillImageFrame = 0x03,
	FormatUncompressed = 0x04,
	FrameUncompressed = 0x05,
	FormatMjpeg = 0x06,
	FrameMjpeg = 0x07,
	ColorFormat = 0x0D,
	FormatFrameBased = 0x10,
	FrameFrameBased = 0x11,
};

/* UVC 1.5 Table B-1: USB Terminal Types and Table B-2: Input Terminal Types, a subset */
enum class TerminalType_t : uint16_t {
	StreamingTerminal = 0x0101,
	CameraSensor = 0x0201,
};

using VideoInterfaceClassCode = detail::constant<ClassCode_t, ClassCode_t::Video>;
using VideoInterfaceProtocol = detail::constant<uint8_t, 0x00>;

template <typename T>
struct __attribute__((__packed__))
VideoDescriptorType : detail::typed<VideoDescriptorType_t> {
	constexpr VideoDescriptorType() : detail::typed<VideoDescriptorType_t>(T::descriptortype()) {}
};

template <typename T>
struct __attribute__((__packed__))
VideoDescriptorSubtype : detail::typed<decltype(T::descriptorsubtype())> {
	constexpr VideoDescriptorSubtype() :
		detail::typed<decltype(T::descriptorsubtype())>(T::descriptorsubtype()) {}
};

template <typename T>
struct __attribute__((__packed__))
VideoInterfaceSubclassCode : detail::constant<VideoInterfaceSubclassCode_t, T::subclass()> {
};

struct __attribute__((__packed__))
Guid {
	uint8_t bytes[16];
};

/* H.264 format GUID {34363248-0000-0010-8000-00AA00389B71}: the FourCC "H264" in the
 * Microsoft media subtype base GUID. The UVC documents do not define guidFormat values
 * (Frame Based Payload Table 3-1) */
inline constexpr Guid GuidH264 = {{0x48, 0x32, 0x36, 0x34, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00,
									0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71}};

/* UVC 1.5 3.7.1 Table 3-2: Standard VC Interface Descriptor, without a status interrupt endpoint */
template <typename UnitCollection, unsigned NStreamingInterfaces = 1>
struct __attribute__((__packed__))
VideoControl {
	using self = VideoControl<UnitCollection, NStreamingInterfaces>;
	using Units = typename UnitCollection::type;

	/* UVC 1.5 3.7.2 Table 3-3: Class-specific VC Interface Header Descriptor */
	struct __attribute__((__packed__))
	Header {
		using self = Header;
		using InCollection = detail::constant<uint8_t, NStreamingInterfaces>;

		static constexpr VideoDescriptorType_t descriptortype() {
			return VideoDescriptorType_t::CS_INTERFACE;
		}
		static constexpr VCInterfaceDescriptorSubtype_t descriptorsubtype() {
			return VCInterfaceDescriptorSubtype_t::Header;
		}
		static constexpr uint8_t length() {
			return sizeof(self);
		}
		static constexpr uint16_t totallength() {
			return sizeof(self) + sizeof(Units);
		}

		Length<self> bLength;
		VideoDescriptorType<self> bDescriptorType;
		VideoDescriptorSubtype<self> bDescriptorSubType;
		BCD bcdUVC;
		TotalLength<self> wTotalLength;
		Number<4> dwClockFrequency;
		InCollection bInCollection;
		InterfaceNumber baInterfaceNr[NStreamingInterfaces];
	};

	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::INTERFACE;
	}
	static constexpr FixedNumber<self> numendpoints() {
		return FixedNumber<self>(0);
	}
	static constexpr VideoInterfaceSubclassCode_t subclass() {
		return VideoInterfaceSubclassCode_t::VideoControl;
	}
	static constexpr uint8_t length() {
		return sizeof(usb2::Interface<Empty>);
	}
	const uint8_t *ptr() const {
		return bLength.ptr();
	}

	Length<self> bLength;
	DescriptorType<self> bDescriptorType;
	InterfaceNumber bInterfaceNumber;
	AlternateSetting bAlternateSetting;
	NumEndpoints<self> bNumEndpoints;
	VideoInterfaceClassCode bInterfaceClass;
	VideoInterfaceSubclassCode<self> bInterfaceSubClass;
	VideoInterfaceProtocol bInterfaceProtocol;
	Index iInterface;
	Header header;
	Units units;
};

/* UVC 1.5 3.7.2.3 Table 3-6: Camera Terminal Descriptor */
template <unsigned ControlSize = 3>
struct __attribute__((__packed__))
CameraTerminal {
	using self = CameraTerminal<ControlSize>;
	using TerminalType = detail::constant<TerminalType_t, TerminalType_t::CameraSensor>;
	using BControlSize = detail::constant<uint8_t, ControlSize>;

	static constexpr VideoDescriptorType_t descriptortype() {
		return VideoDescriptorType_t::CS_INTERFACE;
	}
	static constexpr VCInterfaceDescriptorSubtype_t descriptorsubtype() {
		return VCInterfaceDescriptorSubtype_t::InputTerminal;
	}
	static constexpr uint8_t length() {
		return sizeof(self);
	}

	Length<self> bLength;
	VideoDescriptorType<self> bDescriptorType;
	VideoDescriptorSubtype<self> bDescriptorSubType;
	Number<1> bTerminalID;
	TerminalType wTerminalType;
	Number<1> bAssocTerminal;
	Index iTerminal;
	Number<2> wObjectiveFocalLengthMin;
	Number<2> wObjectiveFocalLengthMax;
	Number<2> wOcularFocalLength;
	BControlSize bControlSize;
	uint8_t bmControls[ControlSize];
};

/* UVC 1.5 3.7.2.2 Table 3-5: Output Terminal Descriptor, towards a USB streaming interface */
struct __attribute__((__packed__))
StreamingOutputTerminal {
	using self = StreamingOutputTerminal;
	using TerminalType = detail::constant<TerminalType_t, TerminalType_t::StreamingTerminal>;

	static constexpr VideoDescriptorType_t descriptortype() {
		return VideoDescriptorType_t::CS_INTERFACE;
	}
	static constexpr VCInterfaceDescriptorSubtype_t descriptorsubtype() {
		return VCInterfaceDescriptorSubtype_t::OutputTerminal;
	}
	static constexpr uint8_t length() {
		return sizeof(self);
	}

	Length<self> bLength;
	VideoDescriptorType<self> bDescriptorType;
	VideoDescriptorSubtype<self> bDescriptorSubType;
	Number<1> bTerminalID;
	TerminalType wTerminalType;
	Number<1> bAssocTerminal;
	Number<1> bSourceID;
	Index iTerminal;
};

/* UVC 1.5 3.9.1 Table 3-13: Standard VS Interface Descriptor, the zero bandwidth alternate setting that carries
 * the class-specific descriptors */
template <unsigned NFormats, typename FormatCollection>
struct __attribute__((__packed__))
VideoStreaming {
	using self = VideoStreaming<NFormats, FormatCollection>;
	using Formats = typename FormatCollection::type;

	/* UVC 1.5 3.9.2.1 Table 3-14: Class-specific VS Interface Input Header Descriptor */
	struct __attribute__((__packed__))
	InputHeader {
		using self = InputHeader;
		using NumFormats = detail::constant<uint8_t, NFormats>;
		using ControlSize = detail::constant<uint8_t, 1>;

		static constexpr VideoDescriptorType_t descriptortype() {
			return VideoDescriptorType_t::CS_INTERFACE;
		}
		static constexpr VSInterfaceDescriptorSubtype_t descriptorsubtype() {
			return VSInterfaceDescriptorSubtype_t::InputHeader;
		}
		static constexpr uint8_t length() {
			return sizeof(self);
		}
		static constexpr uint16_t totallength() {
			return sizeof(self) + sizeof(Formats);
		}

		Length<self> bLength;
		VideoDescriptorType<self> bDescriptorType;
		VideoDescriptorSubtype<self> bDescriptorSubType;
		NumFormats bNumFormats;
		TotalLength<self> wTotalLength;
		EndpointAddress bEndpointAddress;
		Number<1> bmInfo;
		Number<1> bTerminalLink;
		Number<1> bStillCaptureMethod;
		Number<1> bTriggerSupport;
		Number<1> bTriggerUsage;
		ControlSize bControlSize;
		Number<1> bmaControls[NFormats];
	};

	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::INTERFACE;
	}
	static constexpr FixedNumber<self> numendpoints() {
		return FixedNumber<self>(0);
	}
	static constexpr VideoInterfaceSubclassCode_t subclass() {
		return VideoInterfaceSubclassCode_t::VideoStreaming;
	}
	static constexpr uint8_t length() {
		return sizeof(usb2::Interface<Empty>);
	}
	const uint8_t *ptr() const {
		return bLength.ptr();
	}

	Length<self> bLength;
	DescriptorType<self> bDescriptorType;
	InterfaceNumber bInterfaceNumber;
	AlternateSetting bAlternateSetting;
	NumEndpoints<self> bNumEndpoints;
	VideoInterfaceClassCode bInterfaceClass;
	VideoInterfaceSubclassCode<self> bInterfaceSubClass;
	VideoInterfaceProtocol bInterfaceProtocol;
	Index iInterface;
	InputHeader header;
	Formats formats;
};

/* Frame Based Payload 3.1.1 Table 3-1: Frame Based Payload Video Format Descriptor */
struct __attribute__((__packed__))
FrameBasedFormat {
	using self = FrameBasedFormat;

	static constexpr VideoDescriptorType_t descriptortype() {
		return VideoDescriptorType_t::CS_INTERFACE;
	}
	static constexpr VSInterfaceDescriptorSubtype_t descriptorsubtype() {
		return VSInterfaceDescriptorSubtype_t::FormatFrameBased;
	}
	static constexpr uint8_t length() {
		return sizeof(self);
	}

	Length<self> bLength;
	VideoDescriptorType<self> bDescriptorType;
	VideoDescriptorSubtype<self> bDescriptorSubType;
	Number<1> bFormatIndex;
	Number<1> bNumFrameDescriptors;
	Guid guidFormat;
	Number<1> bBitsPerPixel;
	Number<1> bDefaultFrameIndex;
	Number<1> bAspectRatioX;
	Number<1> bAspectRatioY;
	Number<1> bmInterlaceFlags;
	Number<1> bCopyProtect;
	Number<1> bVariableSize;
};

/* Frame Based Payload 3.1.2 Table 3-2: Frame Based Payload Video Frame Descriptor, with
 * discrete frame intervals */
template <unsigned NFrameIntervals = 1>
struct __attribute__((__packed__))
FrameBasedFrame {
	using self = FrameBasedFrame<NFrameIntervals>;
	using FrameIntervalType = detail::constant<uint8_t, NFrameIntervals>;

	static constexpr VideoDescriptorType_t descriptortype() {
		return VideoDescriptorType_t::CS_INTERFACE;
	}
	static constexpr VSInterfaceDescriptorSubtype_t descriptorsubtype() {
		return VSInterfaceDescriptorSubtype_t::FrameFrameBased;
	}
	static constexpr uint8_t length() {
		return sizeof(self);
	}

	Length<self> bLength;
	VideoDescriptorType<self> bDescriptorType;
	VideoDescriptorSubtype<self> bDescriptorSubType;
	Number<1> bFrameIndex;
	Number<1> bmCapabilities;
	Number<2> wWidth;
	Number<2> wHeight;
	Number<4> dwMinBitRate;
	Number<4> dwMaxBitRate;
	Number<4> dwDefaultFrameInterval;
	FrameIntervalType bFrameIntervalType;
	Number<4> dwBytesPerLine;
	Number<4> dwFrameInterval[NFrameIntervals];
};

} // namespace uvc
} // namespace usbplusplus
