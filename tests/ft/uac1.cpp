/* Copyright (C) 2025 Michiel van Leeuwen <michiel@embeddedacoustics.com>
 *
 * test_uac1.cpp - USB++ proof of concept tests for UAC1
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
#include <usbplusplus/uac1.hpp>
#include "ft.hpp"

/****************************************************************************/

using namespace usbplusplus;
using namespace usbplusplus::usb2;
using namespace usbplusplus::uac1;
using namespace usbplusplus::ft;

constexpr ustring sManufacturer = u"MegaCool Corp.";
constexpr ustring sProduct = u"SuperPuper device";
constexpr ustring sInterface = u"Interface";
constexpr ustring sSerialNumber = u"SN-12C55F2";
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

using AudioControlInterface = AudioControl<1,
    List<
        Input_Terminal,
        Feature_Unit<3>,
        Output_Terminal
    >
>;

using SpeakerStreamingInterface = AudioStreaming<List<Type_I_Format_Type<Type_I_DiscreteSampleFrequency<2>>>, List<CS_ASEndpoint, ASEndpoint>>;
using MicrophoneStreamingInterface = AudioStreaming<List<Type_I_Format_Type<Type_I_DiscreteSampleFrequency<2>>>, List<CS_ASEndpoint>>;

using Uac1Configuration = Configuration<
    List<
        AudioControlInterface,
        ZeroBandwidth,
        SpeakerStreamingInterface,
        ZeroBandwidth,
        MicrophoneStreamingInterface
    >
>;

constexpr const AudioControlInterface audio_control_interface = {
    .bInterfaceNumber = 1,
    .bAlternateSetting = 0,
    .header = {
        .bcdADC = 1.00_bcd,
        .baCollection = { 1 },
    },
    .units = {
        Input_Terminal{
            .bTerminalID = 2,
            .wTerminalType = InputTerminalType(InputTerminalType_t::USB_streaming),
            .bAssocTerminal = 0,
            .bNrChannels = 2,
            .wChannelConfig = ChannelConfig(SpatialLocations_t::FL | SpatialLocations_t::FR),
        },
        Feature_Unit<3>{
            .bUnitID = 3,
            .bSourceID = 2,
            .bmaControls = {
                FeatureUnitControls_t::MUTE | FeatureUnitControls_t::VOLUME,
                FeatureUnitControls_t::MUTE | FeatureUnitControls_t::VOLUME,
                FeatureUnitControls_t::MUTE | FeatureUnitControls_t::VOLUME,
            },
        },
        Output_Terminal{
            .bTerminalID = 4,
            .wTerminalType = OutputTerminalType(OutputTerminalType_t::Headphones),
            .bAssocTerminal = 0,
            .bSourceID = 3,
        },
    },
};

constexpr const ZeroBandwidth speaker_interface_alt0 = {
    .bInterfaceNumber = 1,
};

constexpr const SpeakerStreamingInterface speaker_interface = {
    .bInterfaceNumber = 3,
    .bAlternateSetting = 1,
    .header = {
        .bTerminalLink = 2,
        .bDelay = 0,
        .wFormatTag = FormatTag_t::FORMAT_TYPE_I_PCM,
    },
    .formats = {
        {
            .bNrChannels = 2,
            .bSubframeSize = 1,
            .bBitResolution = 16,
            .samfreq = 
                { 
                    .tSamFreq = { 44100, 48000}
                }
        },
    },
    .endpoints = {
        CS_ASEndpoint{
            .endpoint = {
                .bEndpointAddress = EndpointAddress(0x01, EndpointDirection_t::OUT),
                .bmAttributes = Endpoint::Attributes(TransferType_t::Isochronous, SynchronizationType_t::Asynchronous),
                .wMaxPacketSize = 196,
                .bInterval = 1,
                .bRefresh = 0,
                .bSynchAddress = EndpointAddress(0x01, EndpointDirection_t::IN),
            },
            .bmAttributes = CS_ASEndpointAttributes_t::SAMPLING_FREQUENCY,
            .bLockDelayUnits = LockDelayUnits(LockDelayUnits_t::Milliseconds),
            .wLockDelay = 0,
        },
        ASEndpoint{
            .bEndpointAddress = EndpointAddress(0x01, EndpointDirection_t::IN),
            .bmAttributes = Endpoint::Attributes(TransferType_t::Isochronous, SynchronizationType_t::No_Synchronization, UsageType_t::Feedback_endpoint),
            .wMaxPacketSize = 3,
            .bInterval = 1,
            .bRefresh = 2,
            .bSynchAddress = EndpointAddress(0x00, EndpointDirection_t::OUT),
        },
    },
};

constexpr const ZeroBandwidth microphone_interface_alt0 = {
    .bInterfaceNumber = 1,
};

constexpr const MicrophoneStreamingInterface microphone_interface = {
    .bInterfaceNumber = 3,
    .bAlternateSetting = 1,
    .header = {
        .bTerminalLink = 2,
        .bDelay = 0,
        .wFormatTag = FormatTag_t::FORMAT_TYPE_I_PCM,
    },
    .formats = {
        {
            .bNrChannels = 1,
            .bSubframeSize = 1,
            .bBitResolution = 16,
            .samfreq = 
                { 
                    .tSamFreq = { 44100, 48000}
                }
        },
    },
    .endpoints = {
        CS_ASEndpoint{
            .endpoint = {
                .bEndpointAddress = EndpointAddress(0x01, EndpointDirection_t::OUT),
                .bmAttributes = Endpoint::Attributes(TransferType_t::Isochronous, SynchronizationType_t::Synchronous),
                .wMaxPacketSize = 96,
                .bInterval = 1,
                .bRefresh = 0,
                .bSynchAddress = EndpointAddress(0x00, EndpointDirection_t::OUT),
            },
            .bmAttributes = CS_ASEndpointAttributes_t::SAMPLING_FREQUENCY,
            .bLockDelayUnits = LockDelayUnits(LockDelayUnits_t::Milliseconds),
            .wLockDelay = 0,
        },
    },
};

constexpr const Uac1Configuration configuration = {
    .bNumInterfaces = 2,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = ConfigurationCharacteristics_t::Reserved,
    .bMaxPower = 100_mA,
    .interfaces = {
        audio_control_interface,
        speaker_interface_alt0,
        speaker_interface,
        microphone_interface_alt0,
        microphone_interface,
    },
};

static usbdevice uac1 { devaddr::uac1, deviceDescriptor, MyStrings{}, configuration };
