/*
 * Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * tests/ut/main.cpp - main unit tests runner
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "ut.hpp"

int main(int argc, char *argv[]) {
    return boost::ut::cfg<>.run(boost::ut::run_cfg{.argc = argc, .argv = const_cast<const char**>(argv)});
}
