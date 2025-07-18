/* Copyright (C) 2018, 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ft/usb2.cpp - USB++ functional tests
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
#include <usbplusplus/usbplusplus.hpp>
#include <usbplusplus/uac2.hpp>
#include <configurations.hpp>
#include "ft.hpp"

/****************************************************************************/

using namespace usbplusplus;
using namespace usbplusplus::usb2;
using namespace usbplusplus::usb2::tests;
using namespace usbplusplus::ft;

constexpr ustring sManufacturer = u"Test Manufacturer.";
constexpr ustring sProduct      = u"Test Product";
constexpr ustring sInterface    = u"Test Interface";
constexpr ustring sSerialNumber = u"SN-TEST1";
constexpr ustring sManufacturer2 = u"Test Manufacturer Two";
constexpr ustring sProduct2      = u"Test Product Two";
constexpr ustring sInterface2    = u"Interface2";
constexpr ustring sSerialNumber2 = u"SN-TEST2";
constexpr ustring eManufacturer2 = u"Test Manufacturer GB/UK";
constexpr ustring eProduct2      = u"Test Product GB/UK";
constexpr ustring eInterface2    = u"Interface3";
constexpr ustring eSerialNumber2 = u"SN-TEST-UK";
constexpr ustring uManufacturer = u"Тестовий Виробник";
constexpr ustring uProduct      = u"Тестовий Продукт";
using MyStrings = Strings<LanguageIdentifier::English_United_States,
	sManufacturer,
	sProduct,
	sInterface,
	sSerialNumber>;

using Matrix = MultiStrings<
    Strings<LanguageIdentifier::English_United_States,
                                        sManufacturer2, sProduct2, sSerialNumber2, sInterface2>,
    Strings<LanguageIdentifier::English_United_Kingdom,
                                        eManufacturer2, eProduct2, eSerialNumber2, eInterface2>,
    Strings<LanguageIdentifier::Ukrainian,
                                        uManufacturer, uProduct, sSerialNumber2, sInterface>>;

constexpr const Device deviceDescriptor = {
	.bLength = {},
	.bDescriptorType = {},
	.bcdUsb = 1.00_bcd,
	.bDeviceClass = DeviceClass::Defined_in_the_Interface_Descriptors,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = MaxPacketSize0_t::_64,
	.idVendor = 0x0102,
	.idProduct = IDProduct(0x0304),
	.bcdDevice = 1.00_bcd,
	.iManufacturer = MyStrings::indexof(sManufacturer),
	.iProduct = MyStrings::indexof(sProduct),
	.iSerialNumber = MyStrings::indexof(sSerialNumber),
	.bNumConfigurations = 2
};

constexpr const Device deviceDescriptor_2 = {
    .bLength = {},
    .bDescriptorType = {},
    .bcdUsb = 2.00_bcd,
    .bDeviceClass = DeviceClass::Defined_in_the_Interface_Descriptors,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = MaxPacketSize0_t::_64,
    .idVendor = 0x0103,
    .idProduct = IDProduct(0x0001),
    .bcdDevice = 1.00_bcd,
    .iManufacturer = Matrix::indexof(sManufacturer2),
    .iProduct = Matrix::indexof(sProduct2),
    .iSerialNumber = Matrix::indexof(sSerialNumber2),
    .bNumConfigurations = 2
};

constexpr const  Device_Qualifier deviceQualifier = {
	{},
	{},
	1.0_bcd,
	DeviceClass::Defined_in_the_Interface_Descriptors,
	DeviceSubClass(0),
	DeviceProtocol(0),
	MaxPacketSize0_t::_16,
	NumConfigurations(1),
	Reserved<1>()
};

// Creates USB 1.0 device (per deviceDescriptor.bcdUsb)
static usbdevice test1 { devaddr::test1, deviceDescriptor, MyStrings{}, TestUAC2Configuration_1, TestUAC2Configuration_2 };
// Creates USB 2.0 device per deviceDescriptor_2.bcdUsb) with a DeviceQualifier and multilingual strings Matrix.
static usbdevice test2 { devaddr::test2, deviceDescriptor_2, deviceQualifier, Matrix{}, TestUAC2Configuration_2, TestUAC2Configuration_3 };
