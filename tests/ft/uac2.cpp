/* Copyright (C) 2025 Michiel van Leeuwen <michiel@embeddedacoustics.com>
 *
 * test_uac2.cpp - USB++ proof of concept tests for UAC2
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

#pragma GCC diagnostic ignored "-Wmissing-field-initializers" // some field initializers are skipped by intent
#pragma GCC diagnostic ignored "-Wpedantic" // zero-size array member ‘endpoints’ not at end of AudioControl

#include <cstdio>
#include <usbplusplus/usbplusplus.hpp>
#include <usbplusplus/uac2.hpp>
#include "ft.hpp"

/****************************************************************************/

using namespace usbplusplus;
using namespace usbplusplus::usb2;
using namespace usbplusplus::uac2;
using namespace usbplusplus::ft;

constexpr ustring sManufacturer = u"MegaCool Corp.";
constexpr ustring sProduct = u"SuperPuper device";
constexpr ustring sInterface = u"Interface";
constexpr ustring sSerialNumber = u"SN-12C55F2";
constexpr ustring uManufacturer = u"MegaKool Korp.";
constexpr ustring uProduct = u"SuperPuper prystriy";
using MyStrings = Strings<LanguageIdentifier::English_United_States,
                          sManufacturer,
                          sProduct,
                          sInterface,
                          sSerialNumber>;

constexpr const Device deviceDescriptor = {
    .bcdUsb = 2.00_bcd,
    .bDeviceClass = DeviceClass::Defined_in_the_Interface_Descriptors,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = MaxPacketSize0_t::_64,
    .idVendor = 0x0102,
    .idProduct = 0x0304,
    .bcdDevice = 1.00_bcd,
    .iManufacturer = MyStrings::indexof(sManufacturer),
    .iProduct = MyStrings::indexof(sProduct),
    .iSerialNumber = MyStrings::indexof(sSerialNumber),
    .bNumConfigurations = 1,
};

using AudioControlInterface = AudioControl<
    List<
        Clock_Source,
        Input_Terminal,
        Feature_Unit<2>,
        Output_Terminal>,
    None>;
using SpeakerStreamingInterface = AudioStreaming<List<Type_I_Format_Type>, List<AS_Isochronous_Audio_Data_Endpoint>>;
using HeadsetConfiguration = Configuration<
    List<
        usb2::InterfaceAssociation,
        AudioControlInterface,
        ZeroBandwidth,
        SpeakerStreamingInterface>>;

constexpr const usb2::InterfaceAssociation audio_iad = {
    .bFirstInterface = 2,
    .bInterfaceCount = 3,
    .bFunctionClass = usb2::FunctionClass::Audio,
    .bFunctionSubClass = 0,
    .bFunctionProtocol = static_cast<uint8_t>(FunctionProtocol_t::AF_VERSION_02_00),
};

constexpr const AudioControlInterface audio_control_interface = {
    .bInterfaceNumber = 1,
    .bAlternateSetting = 0,
    .header = {
        .bcdADC = 2.00_bcd,
        .bCategory = AudioFunctionCategoryCode_t::HEADSET,
        .bmControls = LatencyControl_t::none,
    },
    .units = {
        Clock_Source{
            .bClockID = 1,
            .bmAttributes = Clock_Source::Attributes(ClockType_t::Internal_fixed_Clock),
            .bmControls = Clock_Source::Controls({
                .frequencyControl = 0,
                .validityControl = 0,
            }),
            .bAssocTerminal = 0,
        },
        Input_Terminal{
            .bTerminalID = 2,
            .wTerminalType = InputTerminalType(InputTerminalType_t::USB_streaming),
            .bAssocTerminal = 0,
            .bCSourceID = 1,
            .bNrChannels = 2,
            .bmChannelConfig = ChannelConfig(SpatialLocations_t::FL | SpatialLocations_t::FR),
            .bmControls = Input_Terminal::Controls(),
        },
        Feature_Unit<2>{
            .bUnitID = 3,
            .bSourceID = 2,
            .bmaControls = {
                Feature_Unit<2>::Controls({
                    .Mute_Control = Control_t::programmable,
                    .Volume_Control = Control_t::programmable,
                }),
                Feature_Unit<2>::Controls({
                    .Mute_Control = Control_t::programmable,
                    .Volume_Control = Control_t::programmable,
                }),
            },
        },
        Output_Terminal{
            .bTerminalID = 4,
            .wTerminalType = OutputTerminalType(OutputTerminalType_t::Headphones),
            .bAssocTerminal = 0,
            .bSourceID = 3,
            .bCSourceID = 1,
            .bmControls = Output_Terminal::Controls(),
        },
        // TODO: Input and output terminal for microphone
    },
};

constexpr const ZeroBandwidth audio_speaker_interface_alt0 = {
    .bInterfaceNumber = 3,
};

constexpr const SpeakerStreamingInterface audio_speaker_interface_alt1 = {
    .bInterfaceNumber = 3,
    .bAlternateSetting = 1,
    .header = {
        .bTerminalLink = 2,
        .bFormatType = FormatTypeCode_t::FORMAT_TYPE_I,
        .bmFormats = AudioDataFormatTypeI_t::PCM,
        .bNrChannels = 2,
        .bmChannelConfig = ChannelConfig(SpatialLocations_t::FL | SpatialLocations_t::FR),
    },
    .formats = {
        {
            .bSubslotSize = 2,
            .bBitResolution = 16,
        },
    },
    .endpoints = {
        AS_Isochronous_Audio_Data_Endpoint{
            .endpoint = {
                .bEndpointAddress = EndpointAddress(0x01, EndpointDirection_t::IN),
                .bmAttributes = Endpoint::Attributes(TransferType_t::Isochronous),
                .wMaxPacketSize = 32,
                .bInterval = 1,
            },
            .bmAttributes = AS_Isochronous_Audio_Data_Endpoint::Attributes(false),
            .bLockDelayUnits = LockDelayUnits(LockDelayUnits_t::Milliseconds),
            .wLockDelay = 1,
        },
    },
};

constexpr const HeadsetConfiguration configuration = {
    .bNumInterfaces = 2,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = ConfigurationCharacteristics_t::Reserved,
    .bMaxPower = 100_mA,
    .interfaces = {
        audio_iad,
        audio_control_interface,
        audio_speaker_interface_alt0,
        audio_speaker_interface_alt1,
    },
};

static usbdevice uac2 { devaddr::uac2, deviceDescriptor, MyStrings{}, configuration };
