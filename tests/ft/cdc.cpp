/* Copyright (C) 2025 Michiel van Leeuwen <michiel@embeddedacoustics.com>
 *
 * test_cdc.cpp - USB++ proof of concept tests for CDC device class
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
#pragma GCC diagnostic ignored "-Wpedantic" // zero-size array member ‘ZeroBandwidth::endpoints’ not at end of CdcConfiguration
#pragma GCC diagnostic ignored "-Wmissing-field-initializers" // some field initializers are skipped by intent

#include <cstdio>
#include <usbplusplus/usbplusplus.hpp>
#include <usbplusplus/cdc.hpp>
#include "ft.hpp"

/****************************************************************************/

using namespace usbplusplus;
using namespace usbplusplus::usb2;
using namespace usbplusplus::cdc;
using namespace usbplusplus::ft;

constexpr ustring sManufacturer = u"MegaCool Corp.";
constexpr ustring sProduct      = u"SuperPuper device";
constexpr ustring sInterface    = u"Interface";
constexpr ustring sSerialNumber = u"SN-12C55F2";
constexpr ustring sMacAddress   = u"123456789ab";
using MyStrings = Strings<LanguageIdentifier::English_United_States,
	sManufacturer,
	sProduct,
	sInterface,
	sSerialNumber,
    sMacAddress>;


constexpr const Device deviceDescriptor = {
	.bLength = {},
	.bDescriptorType = {},
	.bcdUsb = 2.00_bcd,
	.bDeviceClass = DeviceClass::Defined_in_the_Interface_Descriptors,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = DeviceProtocol(0),
	.bMaxPacketSize0 = MaxPacketSize0_t::_64,
	.idVendor = 0x0102,
	.idProduct = IDProduct(0x0304),
	.bcdDevice = 1.00_bcd,
	.iManufacturer = Manufacturer(MyStrings::indexof(sManufacturer)),
	.iProduct = Product(MyStrings::indexof(sProduct)),
	.iSerialNumber = SerialNumber(MyStrings::indexof(sSerialNumber)),
	.bNumConfigurations = NumConfigurations(1)
};

using CdcInterface = Interface<Array<Endpoint,2>>;
using CdcConfiguration =
    Configuration<List<usb2::InterfaceAssociation, CdcEcmControl, ZeroBandwidth, CdcInterface>>;

constexpr const CdcConfiguration configuration = {
	{},
	{},
	{},
	NumInterfaces(2),
	ConfigurationValue(1),
	Index(0),
	CdcConfiguration::Attributes(ConfigurationCharacteristics_t::Remote_Wakeup),
	MaxPower(100_mA),
	{
        usb2::InterfaceAssociation {
            .bFirstInterface = 0,
            .bInterfaceCount = 2,
            .bFunctionClass = usb2::FunctionClass::CDC,
            .bFunctionSubClass = static_cast<uint8_t>(CdcInterfaceSubclassCode_t::EthernetNetworkingControlModel),
            .bFunctionProtocol = static_cast<uint8_t>(CdcInterfaceProtocol_t::None),
        },
        CdcEcmControl {
            .bInterfaceNumber = 0,
            .bAlternateSetting = 0,
            .bInterfaceSubClass = CdcInterfaceSubclassCode_t::EthernetNetworkingControlModel,
            .bInterfaceProtocol = CdcInterfaceProtocol_t::None,
            .iInterface = 0,
            .header =
                {
                    .bcdCDC = 1.10_bcd,
                },
            .functional_descriptors{
                {
                    .bControlInterface = 0,
                    .bSubordinateInterface = { 1 },
                },
                {
                    .iMACAddress = MyStrings::indexof(sMacAddress),
                    .bmEthernetStatistics = EthernetStatistics_t(0),
                    .wMaxSegmentSize = 1514,
                    .wNumberMCFilters = 0,
                    .bNumberPowerFilters = 0,
                },
            },
            .endpoints =
                {
                    Endpoint{
                        .bEndpointAddress = EndpointAddress(1, EndpointDirection_t::IN),
                        .bmAttributes = Endpoint::Attributes(TransferType_t::Interrupt),
                        .wMaxPacketSize = 16,
                        .bInterval = 16,
                    },
                },
        },
        ZeroBandwidth {
            .bInterfaceNumber = 1,
            .bAlternateSetting = 0,
            .bInterfaceSubClass = static_cast<uint8_t>(CdcInterfaceSubclassCode_t::Reseved),
            .bInterfaceProtocol = static_cast<uint8_t>(CdcInterfaceProtocol_t::None),
        },
        CdcInterface {
            .bInterfaceNumber = 1,
            .bAlternateSetting = 1,
            .bInterfaceClass = ClassCode_t::CDC_Data,
            .bInterfaceSubClass = static_cast<uint8_t>(CdcInterfaceSubclassCode_t::Reseved),
            .bInterfaceProtocol = static_cast<uint8_t>(CdcInterfaceProtocol_t::None),
            .endpoints =
                {
                    {
                        .bEndpointAddress = EndpointAddress(2, EndpointDirection_t::OUT),
                        .bmAttributes = Endpoint::Attributes(TransferType_t::Bulk),
                        .wMaxPacketSize = 64,
                        .bInterval = 0,
                    },
                    {
                        .bEndpointAddress = EndpointAddress(2, EndpointDirection_t::IN),
                        .bmAttributes = Endpoint::Attributes(TransferType_t::Bulk),
                        .wMaxPacketSize = 64,
                        .bInterval = 0,
                    },
                },
        },
	}
};

static usbdevice cdc { devaddr::cdc, deviceDescriptor, MyStrings{}, configuration };
