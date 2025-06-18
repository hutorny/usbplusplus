/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/common/configurations.hpp - commonly used configuration descriptors
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */


#pragma once
#include <usbplusplus/usbplusplus.hpp>
#include <usbplusplus/uac2.hpp>

namespace usbplusplus {
namespace usb2 {
namespace tests {

using Interface1 = Interface<Empty>;
using Configuration1 = Configuration<Array<Interface1,2>>;

using Interface2 = Interface<Array<Endpoint,2>>;
using Configuration2 = Configuration<Array<Interface2,2>>;

using Configuration3 = Configuration<List<Interface1, Interface2, Interface2>>;

constexpr const Configuration1 TestUAC2Configuration_1 = {
    {},
    {},
    {},
    NumInterfaces(2),
    ConfigurationValue(1),
    Index(0),
    Configuration1::Attributes(ConfigurationCharacteristics_t::Remote_Wakeup),
    MaxPower(100_mA),
    {
        {
            {},
            {},
            InterfaceNumber(1),
            AlternateSetting(1),
            {},
            InterfaceClass::Audio,
            InterfaceSubClass(0),
            InterfaceProtocol(0),
            Index(0)
        },
        {
            {},
            {},
            InterfaceNumber(2),
            AlternateSetting(1),
            {},
            InterfaceClass::Audio,
            InterfaceSubClass(0),
            InterfaceProtocol(0),
            Index(0)
        }
    }
};

constexpr Endpoint TestEndpoint(uint8_t addr, EndpointDirection_t dir, uint16_t maxPacketSize) {
    return {
        Length<Endpoint>(),
        {},
        EndpointAddress(addr, dir),
        Endpoint::Attributes(TransferType_t::Isochronous),
        MaxPacketSize(maxPacketSize),
        Interval(1)
    };
}

static constexpr InterfaceProtocol AudioInterfaceProtocol_02_00 {
    static_cast<uint8_t>(uac2::AudioInterfaceProtocolCode_t::IP_VERSION_02_00)};

constexpr const Configuration2 TestUAC2Configuration_2 = {
    .bLength = {},
    .bDescriptorType = {},
    .wTotalLength = {},
    .bNumInterfaces = 2,
    .bConfigurationValue = 2,
    .iConfiguration = 0,
    .bmAttributes = {},
    .bMaxPower = 100_mA,
    .interfaces = {
        Interface2 {
            .bLength = {},
            .bDescriptorType = {},
            .bInterfaceNumber = 1,
            .bAlternateSetting = 1,
            .bNumEndpoints = {},
            .bInterfaceClass = InterfaceClass::Audio,
            .bInterfaceSubClass = {},
            .bInterfaceProtocol = AudioInterfaceProtocol_02_00,
            .iInterface = {},
            .endpoints = {
                TestEndpoint(1, EndpointDirection_t::IN, 256),
                TestEndpoint(2, EndpointDirection_t::OUT, 256)
            }
        },
        Interface2 {
            .bLength = {},
            .bDescriptorType = {},
            .bInterfaceNumber = 2,
            .bAlternateSetting = 1,
            .bNumEndpoints = {},
            .bInterfaceClass = InterfaceClass::Audio,
            .bInterfaceSubClass = {},
            .bInterfaceProtocol = AudioInterfaceProtocol_02_00,
            .iInterface = {},
            .endpoints = {
                TestEndpoint(3, EndpointDirection_t::IN, 256),
                TestEndpoint(4, EndpointDirection_t::OUT, 256)
            }
        }
    }
};

constexpr const Configuration3 TestUAC2Configuration_3 = {
    {},
    {},
    {},
    NumInterfaces(3),
    ConfigurationValue(3),
    Index(0),
    Configuration3::Attributes(),
    MaxPower(100_mA),
    {
        {
            {},
            {},
            InterfaceNumber(1),
            AlternateSetting(1),
            {},
            InterfaceClass::Audio,
            InterfaceSubClass(0),
            InterfaceProtocol(0),
            Index(0)
        },
        {
            {},
            {},
            InterfaceNumber(2),
            AlternateSetting(1),
            {},
            InterfaceClass::Audio,
            InterfaceSubClass(0),
            InterfaceProtocol(0),
            {},
            {  TestEndpoint(1, EndpointDirection_t::IN, 256),
               TestEndpoint(2, EndpointDirection_t::OUT, 256) }
        },
        {
            {},
            {},
            InterfaceNumber(3),
            AlternateSetting(1),
            {},
            InterfaceClass::Audio,
            InterfaceSubClass(0),
            InterfaceProtocol(0),
            Index(0),
            {  TestEndpoint(3, EndpointDirection_t::IN, 256),
               TestEndpoint(4, EndpointDirection_t::OUT, 256) }
        }
    }
};

} // namespace tests
} // namespace usb2
} // namespace usbplusplus

