// This file has been generated with Gemini
#pragma once

#include <string_view> // For std::string_view
#include <cstdint>     // For uint8_t, uint16_t
#include <array>       // For std::array
#include <algorithm>   // For std::lower_bound

// --- USB Language Identifier Constants ---
// These constants are derived from the Windows NLS API (WinNls.h) and are
// commonly used for USB Language IDs.

// Primary Language IDs (LANGID low byte)
constexpr uint8_t LANG_ARABIC      = 0x01;
constexpr uint8_t LANG_BULGARIAN   = 0x02;
constexpr uint8_t LANG_CHINESE     = 0x04;
constexpr uint8_t LANG_CZECH       = 0x05;
constexpr uint8_t LANG_DANISH      = 0x06;
constexpr uint8_t LANG_GERMAN      = 0x07;
constexpr uint8_t LANG_GREEK       = 0x08;
constexpr uint8_t LANG_ENGLISH     = 0x09;
constexpr uint8_t LANG_SPANISH     = 0x0A;
constexpr uint8_t LANG_FINNISH     = 0x0B;
constexpr uint8_t LANG_FRENCH      = 0x0C;
constexpr uint8_t LANG_HUNGARIAN   = 0x0E;
constexpr uint8_t LANG_ICELANDIC   = 0x0F;
constexpr uint8_t LANG_ITALIAN     = 0x10;
constexpr uint8_t LANG_JAPANESE    = 0x11;
constexpr uint8_t LANG_KOREAN      = 0x12;
constexpr uint8_t LANG_DUTCH       = 0x13;
constexpr uint8_t LANG_NORWEGIAN   = 0x14;
constexpr uint8_t LANG_POLISH      = 0x15;
constexpr uint8_t LANG_PORTUGUESE  = 0x16;
constexpr uint8_t LANG_ROMANIAN    = 0x18;
constexpr uint8_t LANG_RUSSIAN     = 0x19;
constexpr uint8_t LANG_CROATIAN    = 0x1A; // Used for Serbian too, with different sublanguage
constexpr uint8_t LANG_SLOVAK      = 0x1B;
constexpr uint8_t LANG_SWEDISH     = 0x1D;
constexpr uint8_t LANG_TURKISH     = 0x1F;
constexpr uint8_t LANG_UKRAINIAN   = 0x22;
constexpr uint8_t LANG_SLOVENIAN   = 0x24;
constexpr uint8_t LANG_ESTONIAN    = 0x25;
constexpr uint8_t LANG_LATVIAN     = 0x26;
constexpr uint8_t LANG_LITHUANIAN  = 0x27;
constexpr uint8_t LANG_MALTESE     = 0x3A;
constexpr uint8_t LANG_IRISH       = 0x3C;


// SubLanguage IDs (LANGID high byte, shifted)
// For most primary languages, SUBLANG_DEFAULT or SUBLANG_NEUTRAL is 0x00 or 0x01.
// We use common country-specific sublanguages where applicable for USB.
constexpr uint8_t SUBLANG_NEUTRAL                  = 0x00;
constexpr uint8_t SUBLANG_DEFAULT                  = 0x01; // Often for primary country variant

constexpr uint8_t SUBLANG_ARABIC_SAUDI_ARABIA      = 0x01;
constexpr uint8_t SUBLANG_BULGARIAN_BULGARIA       = 0x01;
constexpr uint8_t SUBLANG_CHINESE_SIMPLIFIED_PRC   = 0x02; // For zh (Simplified Chinese, PRC)
constexpr uint8_t SUBLANG_CZECH_CZECH_REPUBLIC     = 0x01;
constexpr uint8_t SUBLANG_DANISH_DENMARK           = 0x01;
constexpr uint8_t SUBLANG_DUTCH_NETHERLANDS        = 0x01;
constexpr uint8_t SUBLANG_ENGLISH_US               = 0x01;
constexpr uint8_t SUBLANG_FINNISH_FINLAND          = 0x01;
constexpr uint8_t SUBLANG_FRENCH_FRANCE            = 0x01;
constexpr uint8_t SUBLANG_GERMAN_GERMANY           = 0x01;
constexpr uint8_t SUBLANG_GREEK_GREECE             = 0x01;
constexpr uint8_t SUBLANG_HUNGARIAN_HUNGARY        = 0x01;
constexpr uint8_t SUBLANG_ICELANDIC_ICELAND        = 0x01;
constexpr uint8_t SUBLANG_ITALIAN_ITALY            = 0x01;
constexpr uint8_t SUBLANG_JAPANESE_JAPAN           = 0x01;
constexpr uint8_t SUBLANG_KOREAN_KOREA             = 0x01;
constexpr uint8_t SUBLANG_NORWEGIAN_BOKMAL         = 0x01; // Common for Norway
constexpr uint8_t SUBLANG_POLISH_POLAND            = 0x01;
constexpr uint8_t SUBLANG_PORTUGUESE_PORTUGAL      = 0x02;
constexpr uint8_t SUBLANG_PORTUGUESE_BRAZIL        = 0x01;
constexpr uint8_t SUBLANG_ROMANIAN_ROMANIA         = 0x01;
constexpr uint8_t SUBLANG_RUSSIAN_RUSSIA           = 0x01;
constexpr uint8_t SUBLANG_CROATIAN_CROATIA         = 0x01;
constexpr uint8_t SUBLANG_SERBIAN_SERBIA_LATIN     = 0x02; // For sr (Serbian Latin)
constexpr uint8_t SUBLANG_SLOVAK_SLOVAKIA          = 0x01;
constexpr uint8_t SUBLANG_SLOVENIAN_SLOVENIA       = 0x01;
constexpr uint8_t SUBLANG_SPANISH_SPAIN            = 0x01;
constexpr uint8_t SUBLANG_SWEDISH_SWEDEN           = 0x01;
constexpr uint8_t SUBLANG_TURKISH_TURKEY           = 0x01;
constexpr uint8_t SUBLANG_UKRAINIAN_UKRAINE        = 0x01;
constexpr uint8_t SUBLANG_ESTONIAN_ESTONIA         = 0x01;
constexpr uint8_t SUBLANG_LATVIAN_LATVIA           = 0x01;
constexpr uint8_t SUBLANG_LITHUANIAN_LITHUANIA     = 0x01;
constexpr uint8_t SUBLANG_MALTESE_MALTA            = 0x01;
constexpr uint8_t SUBLANG_IRISH_IRELAND            = 0x01;


/**
 * @brief Helper function to construct a USB Language Identifier (LANGID).
 * @param primary The primary language ID (e.g., LANG_ENGLISH).
 * @param sub The sublanguage ID (e.g., SUBLANG_ENGLISH_US).
 * @return A 16-bit USB Language Identifier.
 */
constexpr uint16_t MAKELANGID(uint8_t primary, uint8_t sub) {
    return (static_cast<uint16_t>(sub) << 10) | static_cast<uint16_t>(primary);
}

// --- Lookup Table Structure ---
/**
 * @brief Represents a single entry in the language mapping table.
 */
struct LangEntry {
    std::string_view iso_code;
    uint16_t lang_id;
};

/**
 * @brief A constexpr sorted array mapping ISO 639-1 codes to USB Language Identifiers.
 * The array must be sorted alphabetically by `iso_code` for `std::lower_bound`
 * to work correctly at compile time.
 */
constexpr std::array<LangEntry, 34> lang_map = { // Updated array size to 34
    // Sorted alphabetically by ISO 639-1 code
    LangEntry{"ar", MAKELANGID(LANG_ARABIC,     SUBLANG_ARABIC_SAUDI_ARABIA)},
    LangEntry{"bg", MAKELANGID(LANG_BULGARIAN,  SUBLANG_BULGARIAN_BULGARIA)},
    LangEntry{"cs", MAKELANGID(LANG_CZECH,      SUBLANG_CZECH_CZECH_REPUBLIC)},
    LangEntry{"da", MAKELANGID(LANG_DANISH,     SUBLANG_DANISH_DENMARK)},
    LangEntry{"de", MAKELANGID(LANG_GERMAN,     SUBLANG_GERMAN_GERMANY)},
    LangEntry{"el", MAKELANGID(LANG_GREEK,      SUBLANG_GREEK_GREECE)},
    LangEntry{"en", MAKELANGID(LANG_ENGLISH,    SUBLANG_ENGLISH_US)},
    LangEntry{"es", MAKELANGID(LANG_SPANISH,    SUBLANG_SPANISH_SPAIN)},
    LangEntry{"et", MAKELANGID(LANG_ESTONIAN,   SUBLANG_ESTONIAN_ESTONIA)},
    LangEntry{"fi", MAKELANGID(LANG_FINNISH,    SUBLANG_FINNISH_FINLAND)},
    LangEntry{"fr", MAKELANGID(LANG_FRENCH,     SUBLANG_FRENCH_FRANCE)},
    LangEntry{"ga", MAKELANGID(LANG_IRISH,      SUBLANG_IRISH_IRELAND)},
    LangEntry{"hr", MAKELANGID(LANG_CROATIAN,   SUBLANG_CROATIAN_CROATIA)},
    LangEntry{"hu", MAKELANGID(LANG_HUNGARIAN,  SUBLANG_HUNGARIAN_HUNGARY)},
    LangEntry{"is", MAKELANGID(LANG_ICELANDIC,  SUBLANG_ICELANDIC_ICELAND)},
    LangEntry{"it", MAKELANGID(LANG_ITALIAN,    SUBLANG_ITALIAN_ITALY)},
    LangEntry{"ja", MAKELANGID(LANG_JAPANESE,   SUBLANG_JAPANESE_JAPAN)},
    LangEntry{"ko", MAKELANGID(LANG_KOREAN,     SUBLANG_KOREAN_KOREA)},
    LangEntry{"lt", MAKELANGID(LANG_LITHUANIAN, SUBLANG_LITHUANIAN_LITHUANIA)},
    LangEntry{"lv", MAKELANGID(LANG_LATVIAN,    SUBLANG_LATVIAN_LATVIA)},
    LangEntry{"mt", MAKELANGID(LANG_MALTESE,    SUBLANG_MALTESE_MALTA)},
    LangEntry{"nl", MAKELANGID(LANG_DUTCH,      SUBLANG_DUTCH_NETHERLANDS)},
    LangEntry{"no", MAKELANGID(LANG_NORWEGIAN,  SUBLANG_NORWEGIAN_BOKMAL)},
    LangEntry{"pl", MAKELANGID(LANG_POLISH,     SUBLANG_POLISH_POLAND)},
    LangEntry{"pt", MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_PORTUGAL)},
    LangEntry{"ro", MAKELANGID(LANG_ROMANIAN,   SUBLANG_ROMANIAN_ROMANIA)},
    LangEntry{"ru", MAKELANGID(LANG_RUSSIAN,    SUBLANG_RUSSIAN_RUSSIA)},
    LangEntry{"sk", MAKELANGID(LANG_SLOVAK,     SUBLANG_SLOVAK_SLOVAKIA)},
    LangEntry{"sl", MAKELANGID(LANG_SLOVENIAN,  SUBLANG_SLOVENIAN_SLOVENIA)},
    LangEntry{"sr", MAKELANGID(LANG_CROATIAN,   SUBLANG_SERBIAN_SERBIA_LATIN)}, // Serbian uses LANG_CROATIAN
    LangEntry{"sv", MAKELANGID(LANG_SWEDISH,    SUBLANG_SWEDISH_SWEDEN)},
    LangEntry{"tr", MAKELANGID(LANG_TURKISH,    SUBLANG_TURKISH_TURKEY)},
    LangEntry{"uk", MAKELANGID(LANG_UKRAINIAN,  SUBLANG_UKRAINIAN_UKRAINE)},
    LangEntry{"zh", MAKELANGID(LANG_CHINESE,    SUBLANG_CHINESE_SIMPLIFIED_PRC)},
};

// --- Conversion Function ---
/**
 * @brief Converts an ISO 639-1 two-letter language code to a USB Language Identifier (LANGID).
 * This function is constexpr, meaning it can be evaluated at compile time
 * if the input `iso_code` is a compile-time constant.
 *
 * @param iso_code A std::string_view representing the ISO 639-1 language code (e.g., "en", "fr").
 * @return The corresponding 16-bit USB Language Identifier (LANGID).
 * Defaults to 0x0409 (English - United States) if the code is not found.
 */
inline constexpr uint16_t iso639_to_usb_langid(std::string_view iso_code) {
    // Perform a binary search on the sorted constexpr array.
    // The lambda provides the comparison logic for std::string_view against LangEntry.
    auto it = std::lower_bound(
        lang_map.begin(), lang_map.end(), iso_code,
        [](const LangEntry& entry, std::string_view code) {
            return entry.iso_code < code;
        }
    );

    // Check if the element was found and matches the exact ISO code.
    if (it != lang_map.end() && it->iso_code == iso_code) {
        return it->lang_id;
    }

    return 0;
}
static_assert(iso639_to_usb_langid("en") == 0x409);
static_assert(iso639_to_usb_langid("fr") == 0x40C);
static_assert(iso639_to_usb_langid("de") == 0x407);
static_assert(iso639_to_usb_langid("zh") == 0x804);
static_assert(iso639_to_usb_langid("ja") == 0x411);
static_assert(iso639_to_usb_langid("it") == 0x410);
static_assert(iso639_to_usb_langid("pt") == 0x816);
static_assert(iso639_to_usb_langid("bg") == 0x402);
static_assert(iso639_to_usb_langid("cs") == 0x405);
static_assert(iso639_to_usb_langid("el") == 0x408);
static_assert(iso639_to_usb_langid("hu") == 0x40E);
static_assert(iso639_to_usb_langid("ro") == 0x418);
static_assert(iso639_to_usb_langid("sk") == 0x41B);
static_assert(iso639_to_usb_langid("sl") == 0x424);
static_assert(iso639_to_usb_langid("uk") == 0x422);
static_assert(iso639_to_usb_langid("hr") == 0x41A);
static_assert(iso639_to_usb_langid("sr") == 0x81A);
static_assert(iso639_to_usb_langid("et") == 0x425);
static_assert(iso639_to_usb_langid("lv") == 0x426);
static_assert(iso639_to_usb_langid("lt") == 0x427);
static_assert(iso639_to_usb_langid("ga") == 0x43C);
static_assert(iso639_to_usb_langid("mt") == 0x43A);
static_assert(iso639_to_usb_langid("is") == 0x40F);
static_assert(iso639_to_usb_langid("xx") == 0);

