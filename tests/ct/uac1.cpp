/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ct/uac1.cpp - compile time tests for UAC1 descriptors
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */
#include <usbplusplus/uac1.hpp>

namespace usbplusplus {
namespace uac1 {
namespace tests {
using namespace usb1;

using AudioControlInterface = AudioControl<1,
    List<
        Input_Terminal,
        Feature_Unit<3>,
        Output_Terminal
    >
>;

using SpeakerStreamingInterface = AudioStreaming<
        List<Type_I_Format_Type<Type_I_DiscreteSampleFrequency<2>>>,
        List<AS_Isochronous_Audio_Data_Endpoint, Endpoint>>;

using MicrophoneStreamingInterface = AudioStreaming<
        List<Type_I_Format_Type<Type_I_DiscreteSampleFrequency<2>>>,
        List<AS_Isochronous_Audio_Data_Endpoint>>;

static_assert(AudioControlInterface::length() == 9, "AudioControlInterface::length()");
static_assert(AudioControlInterface::Header::totallength() == 43, "AudioControlInterface::Header::totallength()");
static_assert(SpeakerStreamingInterface::length() == 9, "SpeakerStreamingInterface::length()");
static_assert(MicrophoneStreamingInterface::length() == 9, "MicrophoneStreamingInterface::length()");

} // namespace tests
} // namespace usb1
} // namespace usbplusplus


