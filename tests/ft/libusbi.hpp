/* Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ft/libusbi.hpp - C++ wrapper for libusbi.h
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#pragma once
extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wzero-length-array"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#ifndef _Atomic // for some reason GCC does not define it when build with g++
#define _Atomic(T)  volatile T
#endif
typedef _Atomic(long) atomic_long;

#include "libusbi.h"
#pragma GCC diagnostic pop
}
