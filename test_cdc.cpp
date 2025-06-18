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


#include <cstdio>
#include <clocale>
#include "include/usbplusplus/usbplusplus.hpp"
#include "include/usbplusplus/cdc.hpp"

/****************************************************************************/

using namespace usbplusplus;
using namespace usbplusplus::usb2;
using namespace usbplusplus::cdc;

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
	DeviceClass::Defined_in_the_Interface_Descriptors,
	.bDeviceSubClass = 0,
	DeviceProtocol(0),
	MaxPacketSize0_t::_64,
	.idVendor = 0x0102,
	IDProduct(0x0304),
	.bcdDevice = 1.00_bcd,
	Manufacturer(MyStrings::indexof(sManufacturer)),
	Product(MyStrings::indexof(sProduct)),
	SerialNumber(MyStrings::indexof(sSerialNumber)),
	NumConfigurations(2)
};

using CdcInterface = Interface<Array<Endpoint,2>>;
using CdcConfiguration =
    Configuration<List<usb2::InterfaceAssociation, CdcEcmControl, ZeroBandwidth, CdcInterface>>;

constexpr const CdcConfiguration configuration = {
	.bLength = {},
	.bDescriptorType = {},
	.wTotalLength = {},
	.bNumInterfaces = 2,
	ConfigurationValue(1),
	Index(0),
	CdcConfiguration::Attributes(ConfigurationCharacteristics_t::Remote_Wakeup),
	MaxPower(100_mA),
	{
        (usb2::InterfaceAssociation) {
            .bFirstInterface = 0,
            .bInterfaceCount = 2,
            .bFunctionClass = usb2::FunctionClass::CDC,
            .bFunctionSubClass = (uint8_t)CdcInterfaceSubclassCode_t::EthernetNetworkingControlModel,
            .bFunctionProtocol = (uint8_t)CdcInterfaceProtocol_t::None,
        },
        (CdcEcmControl) {
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
                    .bmEthernetStatistics = (EthernetStatistics_t)0,
                    .wMaxSegmentSize = 1514,
                    .wNumberMCFilters = 0,
                    .bNumberPowerFilters = 0,
                },
            },
            .endpoints =
                {
                    (Endpoint){
                        .bEndpointAddress = EndpointAddress(1, EndpointDirection_t::IN),
                        .bmAttributes = Endpoint::Attributes(TransferType_t::Interrupt),
                        .wMaxPacketSize = 16,
                        .bInterval = 16,
                    },
                },
        },
        (ZeroBandwidth) { 
            .bInterfaceNumber = 1,
            .bAlternateSetting = 0,
            .bInterfaceSubClass = (uint8_t)CdcInterfaceSubclassCode_t::Reseved,
            .bInterfaceProtocol = (uint8_t)CdcInterfaceProtocol_t::None,
        },
        (CdcInterface) {
            .bInterfaceNumber = 1,
            .bAlternateSetting = 1,
            .bInterfaceClass = ClassCode_t::CDC_Data,
            .bInterfaceSubClass = (uint8_t)CdcInterfaceSubclassCode_t::Reseved,
            .bInterfaceProtocol = (uint8_t)CdcInterfaceProtocol_t::None,
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

static void dump(const char* prefix, const uint8_t* data) {
	printf("%s", prefix);
	unsigned len = (data[0] >= 4) &&
			(data[1] == (uint8_t) DescriptorType_t::CONFIGURATION ||
			 data[1] == (uint8_t) DescriptorType_t::OTHER_SPEED ) ?
			* reinterpret_cast<const uint16_t*>(data+2) : data[0];
	for(uint8_t i = 0; i < len; ++i )
		printf("%c 0x%2.2X", (i?',':'{'), data[i]);
	printf(" }\n");
}

int main(int argc, char *argv[]) {
	dump("device:         ", deviceDescriptor.ptr());
	dump("configuration : ", configuration.ptr());
	return 0;
}
