/*
 * Copyright (C) 2026 Daan van Oppenraaij <daan.vo@hotmail.com>
 *
 * tests/ct/uvc.cpp - compile tests for UVC
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include <usbplusplus/usbplusplus.hpp>
#include <usbplusplus/uvc.hpp>

namespace usbplusplus {
namespace uvc {
namespace tests {

using Control = VideoControl<List<CameraTerminal<>, StreamingOutputTerminal>>;
using Streaming = VideoStreaming<1, List<FrameBasedFormat, FrameBasedFrame<>>>;

static_assert(Control::length() == 9, "VideoControl::length()");
static_assert(sizeof(Control::Header) == 13, "VideoControl::Header size");
static_assert(CameraTerminal<>::length() == 18, "CameraTerminal::length()");
static_assert(StreamingOutputTerminal::length() == 9, "StreamingOutputTerminal::length()");
static_assert(Streaming::length() == 9, "VideoStreaming::length()");
static_assert(sizeof(Streaming::InputHeader) == 14, "VideoStreaming::InputHeader size");
static_assert(FrameBasedFormat::length() == 28, "FrameBasedFormat::length()");
static_assert(FrameBasedFrame<>::length() == 30, "FrameBasedFrame::length()");
static_assert(sizeof(GuidH264) == 16, "GuidH264 size");

} // namespace tests
} // namespace uvc
} // namespace usbplusplus
