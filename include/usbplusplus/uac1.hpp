#pragma once
#include "usbplusplus.hpp"

/*
 *
 * Audio10.pdf
 */

namespace usbplusplus {
namespace uac1 {

constexpr BCD Release = 1.0_bcd;

/* 4.1 Audio Channel Cluster Descriptor										 */
/** Spatial Locations														 */
enum class SpatialLocations_t : uint16_t {
	Front_Left				= D(0 ),
	FL						= Front_Left,
	Front_Right				= D(1 ),
	FR						= Front_Right,
	Front_Center			= D(2 ),
	FC						= Front_Center,
	Low_Frequency_Enhancement	= D(3 ),
	LFE						= Low_Frequency_Enhancement,
	Left_Surround			= D(4 ),
	LS						= Left_Surround,
	Right_Surround			= D(5 ),
	RS						= Right_Surround,
	Left_of_Center			= D(6 ),
	LC						= Left_of_Center,
	Right_of_Center			= D(7 ),
	RC						= Right_of_Center,
	Back_Center				= D(8 ),
	BC  					= Back_Center,
	Side_Left				= D(9 ),
	SL  					= Side_Left,
	Side_Right				= D(10),
	SR  					= Side_Right,
	Top						= D(11),
	T	  					= Top,
	//D12..D15: Reserved        =     )
};

enum class SpatialLocationRaw_t : uint32_t {
	/* Mutually exclusive with all other spatial locations */
		Raw_Data 			= D(31),
		RD 					= Raw_Data
};

/* Table A-1: Audio Function Class Code										 */
enum class AudioFunctionClassCode_t : uint8_t {
	AUDIO_FUNCTION	= static_cast<uint8_t>(ClassCode_t::Audio)
};

/* Table A-2: Audio Function Subclass Codes									 */
enum class AudioFunctionSubclassCode_t : uint8_t {
	FUNCTION_SUBCLASS_UNDEFINED	= 0x00
};

/* Table A-3: Audio Function Protocol Codes									 */
enum class FunctionProtocol_t : uint8_t {
	FUNCTION_PROTOCOL_UNDEFINED,
	AF_VERSION_02_00			= 0x20
};

/* Table A-4: Audio Interface Class Code									 */
enum class AudioInterfaceClassCode_t : uint8_t {
	AUDIO = static_cast<uint8_t>(ClassCode_t::Audio)
};

/* Table A-5: Audio Interface Subclass Codes								 */
enum class AudioInterfaceSubclassCode_t : uint8_t {
	INTERFACE_SUBCLASS_UNDEFINED= 0x00,
	AUDIOCONTROL				= 0x01,
	AUDIOSTREAMING				= 0x02,
	MIDISTREAMING				= 0x03
};


/* Table A-6: Audio Interface Protocol Codes								 */
enum class AudioInterfaceProtocolCode_t : uint8_t {
	INTERFACE_PROTOCOL_UNDEFINED= 0x00,
	IP_VERSION_02_00			= 0x20
};

/* Table A-7: Audio Function Category Codes									 */
enum class AudioFunctionCategoryCode_t : uint8_t {
	FUNCTION_SUBCLASS_UNDEFINED = 0x00,
	DESKTOP_SPEAKER				= 0x01,
	HOME_THEATER				= 0x02,
	MICROPHONE					= 0x03,
	HEADSET						= 0x04,
	TELEPHONE					= 0x05,
	CONVERTER					= 0x06,
	VOICE_SOUND_RECORDER		= 0x07,
	I_O_BOX						= 0x08,
	MUSICAL_INSTRUMENT			= 0x09,
	PRO_AUDIO					= 0x0A,
	AUDIO_VIDEO					= 0x0B,
	CONTROL_PANEL 				= 0x0C,
	OTHER						= 0xFF
};

/* Table A-8: Audio Class-specific Descriptor Types							*/
enum class ACDescriptorType_t : uint8_t {
	CS_UNDEFINED				=0x20,
	CS_DEVICE					=0x21,
	CS_CONFIGURATION			=0x22,
	CS_STRING					=0x23,
	CS_INTERFACE				=0x24,
	CS_ENDPOINT					=0x25
};

/* Table A-9: Audio Class-Specific AC Interface Descriptor Subtypes			 */
enum class ACInterfaceDescriptorSubtype_t : uint8_t {
	AC_DESCRIPTOR_UNDEFINED		= 0x00,
	HEADER						= 0x01,
	INPUT_TERMINAL				= 0x02,
	OUTPUT_TERMINAL				= 0x03,
	MIXER_UNIT					= 0x04,
	SELECTOR_UNIT				= 0x05,
	FEATURE_UNIT				= 0x06,
	EFFECT_UNIT					= 0x07,
	PROCESSING_UNIT				= 0x08,
	EXTENSION_UNIT				= 0x09,
	CLOCK_SOURCE				= 0x0A,
	CLOCK_SELECTOR				= 0x0B,
	CLOCK_MULTIPLIER			= 0x0C,
	SAMPLE_RATE_CONVERTER		= 0x0D
};

/* Table A-10: Audio Class-Specific AS Interface Descriptor Subtypes		 */
enum class ASInterfaceDescriptorSubtype_t : uint8_t {
	AS_DESCRIPTOR_UNDEFINED		= 0x00,
	AS_GENERAL					= 0x01,
	FORMAT_TYPE					= 0x02,
	ENCODER						= 0x03,
	DECODER						= 0x04
};

/* Termt20 final.pdf, Table 2-1: USB Terminal Types							*/
enum class USBTerminalType_t : uint16_t {
	USB_Undefined				= 0x0100, /* USB Terminal, undefined Type. */
	USB_streaming				= 0x0101, /* A Terminal dealing with a signal
											carried over an endpoint in an
											AudioStreaming interface.
											The AudioStreaming interface
											descriptor points to the associated
											Terminal through the bTerminalLink
											field. */
	USB_vendor_specific			= 0x01FF, /* A Terminal dealing with a signal
											carried over a vendor-specific
											interface. */
};

/* Termt20 final.pdf, Table 2-2: Input Terminal Types						 */
enum class InputTerminalType_t : uint16_t {
	USB_streaming				= 0x0101,
	Input_Undefined				= 0x0200, /** Input_Terminal, undefined Type.*/
	Microphone					= 0x0201, /** A generic microphone 			 */
	Desktop_microphone			= 0x0202, /** A microphone normally placed on
											the desktop or integrated into the
											monitor.						 */
	Personal_microphone			= 0x0203, /** A head-mounted or clip-on
											microphone.						 */
	Omni_directional_microphone	= 0x0204, /** A microphone designed to pick up
											voice from more than one speaker at
											relatively long ranges.			 */
	Microphone_array			= 0x0205, /** An array of microphones designed
											for directional processing using
											host-based signal processing
											algorithms.						 */
	Processing_microphone_array	= 0x0206, /** An array of microphones with an
											embedded signal processor.		 */
};

/* Termt20 final.pdf, Table 2-3: Output Terminal Types						 */
enum class OutputTerminalType_t : uint16_t {
	USB_streaming				= 0x0101,
	Output_Undefined			= 0x0300, /** Output Terminal, undefined Type*/
	Speaker						= 0x0301, /** A generic speaker 			 */
	Headphones					= 0x0302, /** A head-mounted audio output
											device.*/
	Head_Mounted_Display_Audio	= 0x0303, /** The audio part of a VR head
											mounted display. 				 */
	Desktop_speaker				= 0x0304, /** Relatively small speaker or set of
											speakers normally placed on the
											desktop or integrated into the
											monitor.						*/
	Room_speaker				= 0x0305, /** Larger speaker or set of speakers
											that are heard well anywhere
											in the room.					*/
	Communication_speaker		= 0x0306, /** Speaker or set of speakers
											designed for voice communication. */
	Low_frequency_effects_speaker=0x0307, /** Speaker designed for low
											frequencies (subwoofer). Not capable
											of reproducing speech or music.	 */
};

/* Frmts10.pdf Table A-1, A-2, A-3: Format Type Codes						  */
enum class FormatTag_t : uint16_t {
	FORMAT_TYPE_I_UNDEFINED		= 0x0000,
	FORMAT_TYPE_I_PCM			= 0x0001,
	FORMAT_TYPE_I_PCM8			= 0x0002,
	FORMAT_TYPE_I_IEEE_FLOAT	= 0x0003,
	FORMAT_TYPE_I_IEEE_ALAW		= 0x0004,
	FORMAT_TYPE_I_IEEE_MULOAT	= 0x0005,

	FORMAT_TYPE_II_UNDEFINED	= 0x1000,
	FORMAT_TYPE_II_MPEG			= 0x1001,
	FORMAT_TYPE_II_AC3			= 0x1002,

	FORMAT_TYPE_III_UNDEFINED					= 0x2000,
	FORMAT_TYPE_III_IEC1937_AC3					= 0x2001,
	FORMAT_TYPE_III_IEC1937_MPEG1_LAYER1		= 0x2002,
	FORMAT_TYPE_III_IEC1937_MPEG1_LAYER23		= 0x2003,
	FORMAT_TYPE_III_IEC1937_MPEG2_NOEXT			= 0x2003,
	FORMAT_TYPE_III_IEC1937_MPEG2_EXT			= 0x2004,
	FORMAT_TYPE_III_IEC1937_MPEG2_LAYER1_LS		= 0x2005,
	FORMAT_TYPE_III_IEC1937_MPEG2_LAYER23_LS	= 0x2006,
};

/* Frmts10.pdf Table A-4: Format Type Codes							 */
enum class FormatTypeCode_t : uint8_t {
	FORMAT_TYPE_UNDEFINED		= 0x00,
	FORMAT_TYPE_I				= 0x01,
	FORMAT_TYPE_II				= 0x02,
	FORMAT_TYPE_III				= 0x03,
};

/*****************************************************************************/
/*   AND, OR operators														 */
/*****************************************************************************/
} // namespace uac1

template<> constexpr bool enable_or<uac1::SpatialLocations_t> = true;

namespace uac1 {
/*****************************************************************************/
/*   Field types															 */
/*****************************************************************************/

using InputTerminalType = detail::typed<InputTerminalType_t>;
using OutputTerminalType = detail::typed<OutputTerminalType_t>;

template<typename T>
struct __attribute__((__packed__))
ACDescriptorType : detail::typed<ACDescriptorType_t> {
	constexpr ACDescriptorType() :
		detail::typed<ACDescriptorType_t>(T::descriptortype()) {}
};

template<typename T>
struct __attribute__((__packed__))
ACInterfaceDescriptorSubtype : detail::typed<ACInterfaceDescriptorSubtype_t> {
	constexpr ACInterfaceDescriptorSubtype() :
		detail::typed<ACInterfaceDescriptorSubtype_t>(T::descriptorsubtype()) {}
};

template<typename T>
struct __attribute__((__packed__))
ASInterfaceDescriptorSubtype : detail::typed<ASInterfaceDescriptorSubtype_t> {
	constexpr ASInterfaceDescriptorSubtype() :
		detail::typed<ASInterfaceDescriptorSubtype_t>(T::descriptorsubtype()) {}
};

enum class ReadOnlyControl_t : uint8_t {
	none			= 0b00,
	readonly		= 0b01
};


/**
 * 5.2.4.8 Latency Control
 * An audio function must either not support this Control
 * (D1..0 = 0b00 in the bmControls field of the class-specific
 * AudioControl Interface descriptor) or support this read-only Control for
 * every Terminal and Unit within the audio function (D1..0 = 0b01 in the
 * bmControls field of the class-specific AudioControl Interface descriptor).
 */
using LatencyControl_t = ReadOnlyControl_t;

/** bmControls field contains a set of bit pairs, indicating which
 *  Controls are present at the audio function level (as opposed to at the
 *  addressable entity level) and what their capabilities are.
 *  If a Control is present, it must be Host readable.
 *  If a certain Control is not present then the bit pair must be set to 0b00.
 *  If a Control is present but read-only, the bit pair must be set to 0b01.
 *  If a Control is also Host programmable, the bit pair must be set to 0b11.
 *  The value 0b10 is not allowed.
 */
enum Control_t : uint8_t {
	none			= 0b00,
	readonly		= 0b01,
	programmable	= 0b11,
};


/* TODO Table A-11: Effect Unit Effect Types								 */
/* TODO Table A-12: Processing Unit Process Types							 */

/* Table A-13: Audio Class-Specific Endpoint Descriptor Subtypes			 */
enum class ACEndpointDescriptorSubtype_t : uint8_t {
	DESCRIPTOR_UNDEFINED		= 0x00,
	EP_GENERAL					= 0x01
};

/* Table A-14: Audio Class-Specific Request Codes							 */
enum class ACRequestCode_t : uint8_t {
	REQUEST_CODE_UNDEFINED		= 0x00,
	CUR							= 0x01,
	RANGE						= 0x02,
	MEM							= 0x03
};

/*****************************************************************************/
/*  Table 4-34: Class-Specific AS Isochronous Audio Data Endpoint Descriptor */
enum class LockDelayUnits_t : uint8_t {
	Undefined					= 0,
	Milliseconds				= 1,
	DecodedPCMsamples			= 2
};

/* TODO Table A-15: Encoder Type Codes										*/
/* TODO Table A-16: Decoder Type Codes										*/

using NrChannels = detail::field<1>;
using ChannelConfig = detail::typed<SpatialLocations_t>;

constexpr ChannelConfig StereoChannelConfig = ChannelConfig(
	SpatialLocations_t::Front_Left | SpatialLocations_t::Front_Right);

using None = Empty;

class __attribute__((__packed__))
FunctionClass : detail::typed<AudioFunctionClassCode_t>{
public:
	constexpr FunctionClass() : detail::typed<AudioFunctionClassCode_t>(
			AudioFunctionClassCode_t::AUDIO_FUNCTION) {}
};

class __attribute__((__packed__))
FunctionSubClass : detail::typed<AudioFunctionSubclassCode_t> {
public:
	constexpr FunctionSubClass() : detail::typed<AudioFunctionSubclassCode_t>(
			AudioFunctionSubclassCode_t::FUNCTION_SUBCLASS_UNDEFINED) {}
};

using AudioFunctionClassCode = detail::constant<
	AudioFunctionClassCode_t, AudioFunctionClassCode_t::AUDIO_FUNCTION>;
using AudioFunctionSubclassCode = detail::constant<AudioFunctionSubclassCode_t,
	AudioFunctionSubclassCode_t::FUNCTION_SUBCLASS_UNDEFINED>;
using AudioFunctionCategoryCode = detail::typed<AudioFunctionCategoryCode_t>;

using usb2::InterfaceClass;
using usb2::InterfaceSubClass;

class __attribute__((__packed__))
InterfaceProtocol : ProtocolCode {
public:
	static constexpr ProtocolCode IP_VERSION_NOT_USED = {0x0};
	constexpr InterfaceProtocol() : ProtocolCode(IP_VERSION_NOT_USED) {}
	using ProtocolCode::get;
};

using AudioInterfaceClassCode = detail::constant<AudioInterfaceClassCode_t,
				AudioInterfaceClassCode_t::AUDIO>;

template<typename T>
struct __attribute__((__packed__))
AudioInterfaceSubclassCode :
	public detail::constant<AudioInterfaceSubclassCode_t, T::subclass()> {};

using LockDelayUnits = detail::typed<LockDelayUnits_t>;

struct __attribute__((__packed__))
UnitID : detail::field<1> {
	using typename field<1>::type;
	constexpr UnitID(type value) : detail::field<1>(value) {}
};

struct __attribute__((__packed__))
Delay : detail::field<1> {
	using typename field<1>::type;
	constexpr Delay(type value) : detail::field<1>(value) {}
};


/*****************************************************************************/
/*  Table 4-1: Standard AC Interface Descriptor								 */
/** AudioControl Interface													 */
template<unsigned NInterfaces, typename UnitCollection, typename InterruptEndpoint = None>
struct __attribute__((__packed__))
AudioControl {
	using self = AudioControl<NInterfaces, UnitCollection, InterruptEndpoint>;
	using Endpoints = typename InterruptEndpoint::type;
	using Units = typename UnitCollection::type;

	/*************************************************************************/
	/*  Table 4-2: Class-Specific AC Interface Header Descriptor			 */
	/** Class-Specific AC Interface 										 */
	struct __attribute__((__packed__))
	Header {
		using self = Header;
		using InCollection = detail::constant<uint8_t, NInterfaces>;

		static constexpr ACDescriptorType_t descriptortype() {
			return ACDescriptorType_t::CS_INTERFACE;
		}
		static constexpr ACInterfaceDescriptorSubtype_t descriptorsubtype() {
			return ACInterfaceDescriptorSubtype_t::HEADER;
		}
		static constexpr uint8_t length() {
			return sizeof(self);
		}
		static constexpr uint16_t totallength() {
			return sizeof(Units) + sizeof(self);
		}

		Length<self>				bLength;
		ACDescriptorType<self>		bDescriptorType;
		ACInterfaceDescriptorSubtype<self>	bDescriptorSubType;
		BCD							bcdADC;
		TotalLength<self>			wTotalLength;
		InCollection				bInCollection;
		InterfaceNumber				baCollection[NInterfaces];
	};

	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::INTERFACE;
	}
	static constexpr FixedNumber<self> numendpoints() {
		return FixedNumber<self>(InterruptEndpoint::count);
	}

	static constexpr AudioInterfaceSubclassCode_t subclass() {
		return AudioInterfaceSubclassCode_t::AUDIOCONTROL;
	}
	static constexpr uint8_t length() {
		return sizeof(AudioControl<NInterfaces, None>) - sizeof(Header);
	}
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self>		bDescriptorType;
	InterfaceNumber				bInterfaceNumber;
	AlternateSetting			bAlternateSetting;
	NumEndpoints<self>			bNumEndpoints;
	AudioInterfaceClassCode		bInterfaceClass;
	AudioInterfaceSubclassCode<self> bInterfaceSubClass;
	InterfaceProtocol 			bInterfaceProtocol;
	Index						iInterface;
	Header						header;
	/* The units an dendpoint descriptors follow the interface descriptor*/
	Units						units;
	Endpoints					endpoints;
};

/*****************************************************************************/
/*  Table 4-3: Input Terminal Descriptor									 */
/** Input Terminal 															 */
struct __attribute__((__packed__))
Input_Terminal {
	using self = Input_Terminal;

	static constexpr ACDescriptorType_t descriptortype() {
		return ACDescriptorType_t::CS_INTERFACE;
	}
	static constexpr ACInterfaceDescriptorSubtype_t descriptorsubtype() {
		return ACInterfaceDescriptorSubtype_t::INPUT_TERMINAL;
	}
	static constexpr uint8_t length() {	return sizeof(self); }
	/* ------------------------------------------------*/
	Length<self>				bLength;
	ACDescriptorType<self>		bDescriptorType;
	ACInterfaceDescriptorSubtype<self>	bDescriptorSubType;
	UnitID						bTerminalID;
	InputTerminalType			wTerminalType;
	UnitID						bAssocTerminal;
	Number<1>					bNrChannels;
	ChannelConfig				wChannelConfig;
	Index						iChannelNames;
	Index						iTerminal;
};

/*****************************************************************************/
/*  Table 4-4: Output Terminal Descriptor									 */
/** Output Terminal															 */
struct __attribute__((__packed__))
Output_Terminal {
	using self = Output_Terminal;

	static constexpr ACDescriptorType_t descriptortype() {
		return ACDescriptorType_t::CS_INTERFACE;
	}
	static constexpr ACInterfaceDescriptorSubtype_t descriptorsubtype() {
		return ACInterfaceDescriptorSubtype_t::OUTPUT_TERMINAL;
	}
	static constexpr uint8_t length() {	return sizeof(self); }
	/* ------------------------------------------------*/
	Length<self>				bLength;
	ACDescriptorType<self>		bDescriptorType;
	ACInterfaceDescriptorSubtype<self>	bDescriptorSubType;
	UnitID						bTerminalID;
	OutputTerminalType			wTerminalType;
	UnitID						bAssocTerminal;
	UnitID						bSourceID;
	Index						iTerminal;
};

/* TODO:
 *	Table 4-5: Mixer Unit Descriptor
 *	Table 4-6: Selector Unit Descriptor
 */

struct __attribute__((__packed__))
Feature_Unit_Controls_t {
	unsigned Mute_Control             :2;
	unsigned Volume_Control           :2;
	unsigned Bass_Control             :2;
	unsigned Mid_Control              :2;
	unsigned Treble_Control           :2;
	unsigned Graphic_Equalizer_Control:2;
	unsigned Automatic_Gain_Control   :2;
	unsigned Delay_Control            :2;
	unsigned Bass_Boost_Control       :2;
	unsigned Loudness_Control         :2;
	unsigned Input_Gain_Control       :2;
	unsigned Input_Gain_Pad_Control   :2;
	unsigned Phase_Inverter_Control   :2;
	unsigned Underflow_Control        :2;
	unsigned Overfow_Control          :2;
};

/*****************************************************************************/
/*	Table 4-7: Feature Unit Descriptor										 */
/** Feature_Unit															 */
template<uint8_t ControlsCount>
struct __attribute__((__packed__))
Feature_Unit {
	using self = Feature_Unit<ControlsCount>;
	using Controls = detail::typed<Feature_Unit_Controls_t>;
	using ControlSize = detail::constant<uint8_t, ControlsCount>;

	static constexpr ACDescriptorType_t descriptortype() {
		return ACDescriptorType_t::CS_INTERFACE;
	}
	static constexpr ACInterfaceDescriptorSubtype_t descriptorsubtype() {
		return ACInterfaceDescriptorSubtype_t::FEATURE_UNIT;
	}
	static constexpr uint8_t length() {	return sizeof(self); }
	/* ------------------------------------------------*/
	Length<self>				bLength;
	ACDescriptorType<self>		bDescriptorType;
	ACInterfaceDescriptorSubtype<self>	bDescriptorSubType;
	UnitID						bUnitID;
	UnitID						bSourceID;
	ControlSize					bControlSize;
	Controls					bmaControls[ControlsCount];
	Index						iFeature;
};

/* TODO:
 *	Table 4-14: Sampling Rate Converter Unit Descriptor
 *	Table 4-15: Common Part of the Effect Unit Descriptor
 *	Table 4-16: Parametric Equalizer Section Effect Unit Descriptor
 *	Table 4-17: Reverberation Effect Unit Descriptor
 *	Table 4-18: Modulation Delay Effect Unit Descriptor
 *	Table 4-19: Dynamic Range Compressor Effect Unit Descriptor
 *	Table 4-20: Common Part of the Processing Unit Descriptor
 *	Table 4-21: Up/Down-mix Processing Unit Descriptor
 *	Table 4-22: Dolby Prologic Processing Unit Descriptor
 *	Table 4-23: Stereo Extender Processing Unit Descriptor
 *	Table 4-24: Extension Unit Descriptor
 */

/*****************************************************************************/
/*  Table 4-18: Standard AS Interface Descriptor							 */
/** AudioStreaming Interface												 */
template<typename FormatCollection, typename EndpointCollection>
struct __attribute__((__packed__))
AudioStreaming {
	using self = AudioStreaming<FormatCollection, EndpointCollection>;
	using Formats = typename FormatCollection::type;
	using Endpoints = typename EndpointCollection::type;

	/*****************************************************************************/
	/*  Table 4-19: Class-Specific AS Interface Descriptor						 */
	/** AS Interface												 			*/
	struct __attribute__((__packed__))
	Header {
		using self = Header;
	
		static constexpr ACDescriptorType_t descriptortype() {
			return ACDescriptorType_t::CS_INTERFACE;
		}
		static constexpr ASInterfaceDescriptorSubtype_t descriptorsubtype() {
			return ASInterfaceDescriptorSubtype_t::AS_GENERAL;
		}
		static constexpr uint8_t length() {
			return sizeof(self);
		}
		const uint8_t* ptr() const { return bLength.ptr(); }

		/* ------------------------------------------------*/
		Length<self>				bLength;
		ACDescriptorType<self>		bDescriptorType;
		ASInterfaceDescriptorSubtype<self>	bDescriptorSubType;
		UnitID						bTerminalLink;
		Delay						bDelay;
		FormatTag_t					wFormatTag;
	};

	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::INTERFACE;
	}
	static constexpr FixedNumber<self> numendpoints() {
		return FixedNumber<self>(EndpointCollection::count);
	}
	static constexpr AudioInterfaceSubclassCode_t subclass() {
		return AudioInterfaceSubclassCode_t::AUDIOSTREAMING;
	}
	static constexpr uint8_t length() {
		return sizeof(AudioStreaming<Empty, Empty>) - sizeof(Header);
	}
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self>		bDescriptorType;
	InterfaceNumber				bInterfaceNumber;
	AlternateSetting			bAlternateSetting;
	NumEndpoints<self>			bNumEndpoints;
	AudioInterfaceClassCode		bInterfaceClass;
	AudioInterfaceSubclassCode<self> bInterfaceSubClass;
	InterfaceProtocol 			bInterfaceProtocol;
	Index						iInterface;
	Header						header;
	/* AS Format Type descriptor follows the class-specific descriptor*/
	Formats						formats;
	/* the endpoint descriptors follow the interface descriptor 			 */
	Endpoints					endpoints;
};

/*****************************************************************************/
/*  Table 4-20: Class-Specific AS Isochronous Audio Data Endpoint Descriptor */
/** AS Isochronous Audio Data Endpoint										 */
struct __attribute__((__packed__))
AS_Isochronous_Audio_Data_Endpoint {
	using self = AS_Isochronous_Audio_Data_Endpoint;
	using Refresh = detail::constant<uint8_t, 0>;

	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::ENDPOINT;
	}
	static constexpr uint8_t length() { return sizeof(self); }
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self>		bDescriptorType;
	EndpointAddress				bEndpointAddress;
	usb2::Endpoint::Attributes		bmAttributes;
	MaxPacketSize				wMaxPacketSize;
	Interval					bInterval;
	Refresh						bRefresh;
	EndpointAddress				bSynchAddress;
};

/*****************************************************************************/
/*  Table 4-26: Standard AS Interface Descriptor							 */
/** ZeroBandwidth AudioStreaming Interface									 */
struct __attribute__((__packed__))
ZeroBandwidth {
	using self = ZeroBandwidth;
	using AlternateSetting = detail::constant<uint8_t, 0>;
	using AudioInterfaceSubclassCode = detail::constant<
		AudioInterfaceSubclassCode_t,
		AudioInterfaceSubclassCode_t::AUDIOSTREAMING>;
	static constexpr DescriptorType_t descriptortype() {
		return DescriptorType_t::INTERFACE;
	}
	static constexpr FixedNumber<self> numendpoints() {
		return FixedNumber<self> {0};
	}

	static constexpr uint8_t length() {
		return sizeof(self);
	}
	const uint8_t* ptr() const { return bLength.ptr(); }

	/* ------------------------------------------------*/
	Length<self>				bLength;
	DescriptorType<self>		bDescriptorType;
	InterfaceNumber				bInterfaceNumber;
	AlternateSetting			bAlternateSetting;
	NumEndpoints<self>			bNumEndpoints;
	AudioInterfaceClassCode		bInterfaceClass;
	AudioInterfaceSubclassCode	bInterfaceSubClass;
	InterfaceProtocol 			bInterfaceProtocol;
	Index						iInterface;
};


/*****************************************************************************/
/*  Frmts10.pdf, Table 2-1: Type I Format Type Descriptor					 */
/** Type I Format Type														 */
template <typename SampleFrequencies>
struct __attribute__((__packed__))
Type_I_Format_Type {
	using self = Type_I_Format_Type;
	static constexpr FormatTypeCode_t format_type = FormatTypeCode_t::FORMAT_TYPE_I;
	using FormatTypeCode = detail::constant<FormatTypeCode_t,format_type>;
	using SamFreqType = detail::constant<uint8_t, SampleFrequencies::sam_freq_type()>;

	static constexpr ACDescriptorType_t descriptortype() {
		return ACDescriptorType_t::CS_INTERFACE;
	}
	static constexpr ASInterfaceDescriptorSubtype_t descriptorsubtype() {
		return ASInterfaceDescriptorSubtype_t::FORMAT_TYPE;
	}
	static constexpr uint8_t length() {
		return sizeof(self);
	}

	/* ------------------------------------------------*/
	Length<self>				bLength;
	ACDescriptorType<self>		bDescriptorType;
	ASInterfaceDescriptorSubtype<self> bDescriptorSubtype;
	FormatTypeCode				bFormatType;
	Number<1>					bNrChannels;
	Number<1>					bSubframeSize;
	Number<1>					bBitResolution;
	SamFreqType					bSamFreqType;
	SampleFrequencies			samfreq;
};

class __attribute__((__packed__))
SampleFrequency {
protected:
	uint8_t value[3];
	constexpr const uint8_t* ptr() const { return &value[0]; }
public:
	constexpr uint32_t get() const { 
		return (value[0] << 0) | (value[1] << 8) | (value[2] << 16);
	}
	constexpr SampleFrequency(uint32_t v) : value {
		(uint8_t)((v & 0x0000ff) >> 0),
		(uint8_t)((v & 0x00ff00) >> 8),
		(uint8_t)((v & 0xff0000) >> 16),
	 } {}
};


/*****************************************************************************/
/*  Frmts10.pdf, Table 2-2: Continuous Sampling Frequency					 */
/** Type I Continuous Sampling Frequencyc									 */
struct __attribute__((__packed__))
Type_I_ContinuousSampleFrequency
{
	SampleFrequency					tLowerSamFreq;
	SampleFrequency					tUpperSamFreq;

	static constexpr uint8_t sam_freq_type() {
		return 0;
	}
};

/*****************************************************************************/
/*  Frmts10.pdf, Table 2-3: Discrete Number of Sampling Frequencies			 */
/** Type I Discrete Number of Sampling Frequencies							 */
template<size_t NSampleFrequencies>
struct __attribute__((__packed__))
Type_I_DiscreteSampleFrequency
{
	SampleFrequency					tSamFreq[NSampleFrequencies];

	static constexpr uint8_t sam_freq_type() {
		return NSampleFrequencies;
	}
};

}}
