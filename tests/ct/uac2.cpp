/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ct/uac2.cpp - compile time tests for UAC1 descriptors
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */
#include <usbplusplus/uac2.hpp>

namespace usbplusplus {
namespace uac2 {
namespace tests {
using namespace usb2;

using AudioControlInterface = AudioControl<
    List<
        Clock_Source,
        Input_Terminal,
        Feature_Unit<2>,
        Output_Terminal>,
    None>;

using SpeakerStreamingInterface = AudioStreaming<
        List<Type_I_Format_Type>,
        List<AS_Isochronous_Audio_Data_Endpoint>>;

static_assert(AudioControlInterface::length() == 9, "AudioControlInterface::length()");
static_assert(AudioControlInterface::Header::totallength() == 60, "AudioControlInterface::Header::totallength()");
static_assert(SpeakerStreamingInterface::length() == 9, "SpeakerStreamingInterface::length()");

} // namespace tests
} // namespace usb1
} // namespace usbplusplus


