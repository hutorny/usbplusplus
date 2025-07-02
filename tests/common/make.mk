# Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
#
# tests/comman/make.mk - commonly used make variables
#
#Licensed under MIT License, see full text in LICENSE
#or visit page https://opensource.org/license/mit/

INCLUDES = include tests/common 
BUILDDIR = build
CFLAGS += -O2 $(INCLUDES:%=-I$(PROJROOT)%)
CXXFLAGS += $(STD:%=-std=%) $(CFLAGS) $(WARNINGS) $(if $(findstring gcc, $(CXX)), $(GCCWARN))
WARNINGS = -pedantic -Werror -Wall -Wextra -Wconversion -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization \
           -Wmissing-declarations -Wmissing-include-dirs  -Wold-style-cast -Woverloaded-virtual -Wredundant-decls \
           -Wshadow -Wsign-conversion -Wsign-promo -Wswitch-default -Wundef -Weffc++ -Wfloat-equal \
           -Wno-c++20-extensions -Wno-c++17-extensions
GCCWARN = -Wlogical-op -Wnoexcept -Wstrict-null-sentinel
