/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * test/common/strings.hpp - commonly used strings
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once
#include <usbplusplus/usbplusplus.hpp>

namespace usbplusplus {
namespace usb1 {
namespace tests {

constexpr ustring sManufacturer = u"MegaCool Corp.";
constexpr ustring sProduct      = u"SuperPuper device";
constexpr ustring sInterface    = u"Interface";
constexpr ustring sSerialNumber = u"SN-12C55F2";
constexpr ustring eProduct      = u"SuperDuper device";
constexpr ustring uManufacturer = u"СуперКрута Корп";
constexpr ustring uProduct      = u"СуперПупер пристрій";

using TestStrings = Strings<LanguageIdentifier::English_United_States,
    sManufacturer,
    sProduct,
    sInterface,
    sSerialNumber>;


using TestMultiStrings = MultiStrings<
    Strings<LanguageIdentifier::English_United_States,
                                        sManufacturer, sProduct, sInterface>,
    Strings<LanguageIdentifier::English_United_Kingdom,
                                        sManufacturer, eProduct, sInterface>,
    Strings<LanguageIdentifier::Ukrainian,
                                        uManufacturer, uProduct, sInterface>>;
} // namespace tests
} // namespace usb1
} // namespace usbplusplus

