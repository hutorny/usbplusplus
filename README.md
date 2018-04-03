# USB++

`USB++` (**usbplusplus**) is a C++14 template library that gives the user a 
simple and error-prone way for defining USB descriptors, suitable for 
embedded projects. Plerase visit http://hutorny.in.ua/projects/usbplusplus for
more details.

## Usage

`USB++` is a header-only library. To use include `usbplusplus.hpp`, 
instantiate and initialize descriptors defined in the library.

## Fixed length descriptors 

Define a constant variable of a descriptor type and provide an initializer,
as in this example:
```
#include "usbplusplus.hpp"
using namespace usbplusplus;
using namespace usbplusplus::usb2;

const Device deviceDescriptor = {
	.bLength 		= {},
	.bDescriptorType 	= {},
	.bcdUsb 		= 2.00_bcd,
	.bDeviceClass 	= DeviceClass::Defined_in_the_Interface_Descriptors,
	.bDeviceSubClass 	= 0,
	.bDeviceProtocol 	= 0,
	.bMaxPacketSize0 	= MaxPacketSize0_t::_64,
	.idVendor 		= 0x0102,
	.idProduct 		= 0x0304,
	.bcdDevice 		= 1.00_bcd,
	.iManufacturer 	= 0,
	.iProduct 		= 0,
	.iSerialNumber 	= 0,
	.bNumConfigurations = 1 
};
```
<details><summary>Or, as in this (click to expand):</summary>

```
const Device deviceDescriptor = {
	Length<Device>(),
	DescriptorType<Device>(),
	2.00_bcd,
	DeviceClass::Defined_in_the_Interface_Descriptors,
	DeviceSubClass(0),
	DeviceProtocol(0),
	MaxPacketSize0_t::_64,
	IDVendor(0x0102),
	IDProduct(0x0304),
	1.00_bcd,
	Manufacturer(0),
	Product(0),
	SerialNumber(0),
	NumConfigurations(1)
};
```
</details>

**Note:** fields `bLength` and `bDescriptor` are initialized with the default
value.

## Variable length descriptors 

Define the descriptor type with a template, such as `Interface` or 
`Configuration`, providing types of nested descriptors via variadic template
`List`, plain template `Array`, or stub `Empty`. Instantiate a descriptor 
of that time and provide initializer. 

<details><summary>For example:</summary>
 
```
#include "usbplusplus.hpp"
using namespace usbplusplus;
using namespace usbplusplus::usb2;
using MyInterface1 = Interface<Empty>;
using MyInterface2 = Interface<List<Endpoint>>;

const Configuration<List<MyInterface1,MyInterface2>> myConfiguration = {
		.bLength = {},
		.bDescriptorType = {},
		.wTotalLength = {},
		.bNumInterfaces = {},
		.bConfigurationValue = 1,
		.iConfiguration = 1,
		.bmAttributes = ConfigurationCharacteristics_t::Self_powered,
		.bMaxPower = 100_mA,
		{{
			.bLength 		= {},
			.bDescriptorType 	= {},
			.bInterfaceNumber	= 0,
			.bAlternateSetting	= 0,
			.bNumEndpoints 	= {},
			InterfaceClass::CDC,
			.bInterfaceSubClass = 0,
			.bInterfaceProtocol = 0,
			.iInterface 		= 0
		},
		{
			.bLength 		= {},
			.bDescriptorType	= {},
			.bInterfaceNumber	= 1,
			.bAlternateSetting	= 0,
			.bNumEndpoints	= {},
			InterfaceClass::CDC,
			.bInterfaceSubClass	= 0,
			.bInterfaceProtocol	= 0,
			.iInterface 		= 1,
			{{
				.bLength	= {},
				.bDescriptorType= {},
				EndpointAddress(1, EndpointDirection_t::IN),
				.bmAttributes		= TransferType_t::Isochronous,
				.wMaxPacketSize	= 512,
				.bInterval		= 1,
			}}
		}}
};
```
</details>

**Note:** nested items are enclosed in with outer curve brackets `{ }`. 
The examples emphasize it with `{{ …  }}`

## Replicating descriptors

Sometimes, there is a need to define a sequence of similar descriptors that
differs only in some fields. With C++ this task can be accomplished with a 
consexpr factory function, returning descriptors.
<details><summary>For example:</summary>
 
```
constexpr MyInterface2 myInterface(InterfaceNumber interfaceNumber,
		uint8_t endpointNumber) {
	return  MyInterface2 {
		.bLength 			= {},
		.bDescriptorType	= {},
		.bInterfaceNumber	= interfaceNumber,
		.bAlternateSetting	= 1,
		.bNumEndpoints = {},
		InterfaceClass::CDC,
		.bInterfaceSubClass = 0,
		.bInterfaceProtocol = 0,
		.iInterface 		= MyStrings::indexof(sInterface),
		{{
			.bLength		= {},
			.bDescriptorType= {},
			EndpointAddress(endpointNumber, EndpointDirection_t::IN),
			.bmAttributes = TransferType_t::Isochronous,
			.wMaxPacketSize = 512,
			.bInterval		= 1,
		}}
	};
}
```
</details>

## Descriptor data

Descriptor data is available via method `ptr()` that returns pointer to the
first byte of the descriptor - bLength field.

## String resources

All strings in the device has to be declared upfront, as constexpr `ustring`
variables;
<br>For example:

```
#include "usbplusplus.hpp"
using namespace usbplusplus;
using namespace usbplusplus::usb2;

constexpr ustring sManufacturer = u"MegaCool Corp.";
constexpr ustring sProduct      = u"SuperPuper device";
constexpr ustring sInterface    = u"Interface";
constexpr ustring sSerialNumber = u"SN-12C55F2";
```

Once the strings are declared, they can be used in the definition of string 
resources.

### Monolingual string resources

If support for multiple languages is not needed, the string resources can be 
defined with the `Strings` template. 
<br>For example:

```
using MyStrings = Strings<LanguageIdentifier::English_United_States,
	sManufacturer,
	sProduct,
	sInterface,
	sSerialNumber>;
```

### Multilingual resources

Define strings for every language the same way:

```
constexpr ustring deManufacturer = u"MegaKool Korp.";
constexpr ustring deProduct      = u"SuperPuper Gerät";
constexpr ustring deInterface    = u"Das Interface";
constexpr ustring uaManufacturer = u"МегаКруть корпорація";
constexpr ustring uaProduct      = u"СуперПупер пристрій";
constexpr ustring uaInterface    = u"Інтерфейс";
```

Then define the string resources with `MultiStrings` and `Strings` 
templates, as in the following example:

```
using MyStrings = MultiStrings<
	Strings<LanguageIdentifier::English_United_States,
					sManufacturer,  sProduct,  sInterface,  sSerialNumber>,
	Strings<LanguageIdentifier::English_United_Kingdom,
					sManufacturer,  sProduct,  sInterface,  sSerialNumber>,
	Strings<LanguageIdentifier::English_Canadian,
					sManufacturer,  sProduct,  sInterface,  sSerialNumber>,
	Strings<LanguageIdentifier::German_Standard,
					deManufacturer, deProduct, deInterface, sSerialNumber>,
	Strings<LanguageIdentifier::Ukrainian,
					uaManufacturer, uaProduct, uaInterface, sSerialNumber>
>;
```

where each `Strings` defines resources for a given language.

### String index

Both `Strings` and `MultiStrings` templates implement indexof method that 
returns one-based index of a string, passed as a parameter. 
<br>Note `MultiStrings` just forwards the call to the first `Strings` in 
the list.

### String descriptor

Both `Strings` and `MultiStrings` templates implement method 
`get(index, lang)` that returns a pointer to the string descriptor. 
If index is equal zero, get returns pointer String Descriptor Zero with a 
list of supported languages.
