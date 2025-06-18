/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ut/strings.cpp - unit tests for strings
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "strings.hpp"
#include "ut.hpp"

using namespace usbplusplus;
using namespace usbplusplus::usb2;
using namespace usbplusplus::ut;
using namespace usbplusplus::usb1::tests;
using namespace boost::ut;

namespace {

suite<"Strings"> strings_suite = [] {
    "Manufacturer"_test = [] {
        expect(eq(TestStrings::get(1), u"MegaCool Corp."));
    };
    "Product"_test = [] {
        expect(eq(TestStrings::get(2), u"SuperPuper device"));
    };
    "Out-of-bounds"_test = [] {
        static constexpr const uint8_t* null{};
        expect(eq(TestStrings::get(100), null));
    };
    "Language List"_test = [] {
        expect(eq(TestMultiStrings::get(0, LanguageIdentifier::English_United_States),
                  bytes<8>{ 0x08, 0x03, 0x09, 0x04, 0x09, 0x08, 0x22, 0x04 }));
    };
    "Manufacturer in default language"_test = [] {
        expect(eq(TestMultiStrings::get(1, LanguageIdentifier::English_United_Kingdom), u"MegaCool Corp."));
    };
    "Manufacturer in Ukrainian"_test = [] {
        expect(eq(TestMultiStrings::get(1, LanguageIdentifier::Ukrainian), u"СуперКрута Корп"));
    };
    "Manufacturer in English US"_test = [] {
        expect(eq(TestMultiStrings::get(2, LanguageIdentifier::English_United_States), u"SuperPuper device"));
    };
    "Manufacturer in English UK"_test = [] {
        expect(eq(TestMultiStrings::get(2, LanguageIdentifier::English_United_Kingdom), u"SuperDuper device"));
    };
    "Product in Ukrainian"_test = [] {
        expect(eq(TestMultiStrings::get(2, LanguageIdentifier::Ukrainian), u"СуперПупер пристрій"));
    };
    "Manufacturer in default language"_test = [] {
        expect(eq(TestMultiStrings::get(2, LanguageIdentifier::English_Canadian), u"SuperPuper device"));
    };
};

}

