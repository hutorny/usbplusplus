/* Copyright (C) 2018 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * test.cpp - USB++ proof of concept tests
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

/****************************************************************************/

using namespace usbplusplus;
using namespace usbplusplus::usb2;
using namespace usbplusplus::usb2::tests;

constexpr ustring sManufacturer = u"MegaCool Corp.";
constexpr ustring sProduct      = u"SuperPuper device";
constexpr ustring sInterface    = u"Interface";
constexpr ustring sSerialNumber = u"SN-12C55F2";
constexpr ustring uManufacturer = u"MegaKool Korp.";
constexpr ustring uProduct      = u"SuperPuper prystriy";
using MyStrings = Strings<LanguageIdentifier::English_United_States,
	sManufacturer,
	sProduct,
	sInterface,
	sSerialNumber>;


constexpr const Device deviceDescriptor = {
	.bLength = {},
	.bDescriptorType = {},
	.bcdUsb = 2.00_bcd,
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

constexpr const Languages<2> Languages2 = {
	{},
	{},
	{
		LanguageIdentifier::English_United_States,
		LanguageIdentifier::Ukrainian
	}
};

constexpr const auto Languages3 = LanguageList<
	LanguageIdentifier::English_United_States,
	LanguageIdentifier::English_United_Kingdom,
	LanguageIdentifier::Ukrainian
>();

using Matrix = MultiStrings<
	Strings<LanguageIdentifier::English_United_States,
										sManufacturer, sProduct, sInterface>,
	Strings<LanguageIdentifier::English_United_Kingdom,
										sManufacturer, sProduct, sInterface>,
	Strings<LanguageIdentifier::Ukrainian,
										uManufacturer, uProduct, sInterface>>;

static void uprint(const uint8_t* data) {
	printf("Len: %d, Type %d, String: '", data[0], data[1]);
	const char16_t* chr= reinterpret_cast<const char16_t*>(&data[2]);
	for(unsigned i = data[0]/2-1; i != 0; --i, ++chr) {
		wchar_t lc = *chr;
		printf("%lc", lc);
	}
	printf("'\n");
}

static void dump(const char* prefix, const uint8_t* data) {
	printf("%s", prefix);
	unsigned len = (data[0] >= 4) &&
			(data[1] == static_cast<uint8_t>(DescriptorType_t::CONFIGURATION) ||
			 data[1] == static_cast<uint8_t>(DescriptorType_t::OTHER_SPEED) ) ?
			* reinterpret_cast<const uint16_t*>(data+2) : data[0];
	for(uint8_t i = 0; i < len; ++i )
		printf("%c 0x%2.2X", (i?',':'{'), data[i]);
	printf(" }\n");
}

int main(int, char *[]) {
	setlocale(LC_ALL, "");
	printf("indexof test: %d, %d, %d, %d, %d\n",
		MyStrings::indexof(sManufacturer),
		MyStrings::indexof(sProduct),
		MyStrings::indexof(sInterface),
		MyStrings::indexof(sSerialNumber),
		MyStrings::indexof(uProduct) /* not found */
	);
	printf("indexof test: %d, %d, %d, %d, %d\n",
		Matrix::indexof(sManufacturer),
		Matrix::indexof(sProduct),
		Matrix::indexof(sInterface),
		Matrix::indexof(sSerialNumber),
		Matrix::indexof(uProduct) /* not found */
	);

	dump("device:         ", deviceDescriptor.ptr());
	dump("qualifier:      ", deviceQualifier.ptr());
	dump("configuration1: ", TestUAC2Configuration_1.ptr());
	dump("configuration2: ", TestUAC2Configuration_2.ptr());
	dump("configuration3: ", TestUAC2Configuration_3.ptr());
	uprint(MyStrings::get(1));
	uprint(MyStrings::get(2));
	uprint(MyStrings::get(3));
	uprint(MyStrings::get(4));
	uprint(Matrix::get(Matrix::indexof(sManufacturer), LanguageIdentifier::English_United_Kingdom));
	uprint(Matrix::get(Matrix::indexof(sProduct), LanguageIdentifier::English_United_Kingdom));
	uprint(Matrix::get(Matrix::indexof(sManufacturer), LanguageIdentifier::Ukrainian));
	uprint(Matrix::get(Matrix::indexof(sProduct), LanguageIdentifier::Ukrainian));
	uprint(Matrix::get(Matrix::indexof(sProduct), LanguageIdentifier::Albanian));
	return 0;
}
