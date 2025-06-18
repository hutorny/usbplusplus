# Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
#
# tests/comman/make.mk - commonly used make variables
#
#Licensed under MIT License, see full text in LICENSE
#or visit page https://opensource.org/license/mit/

INCLUDES = include tests/common 
BUILDDIR = build
CXXFLAGS += $(STD:%=-std=%) $(INCLUDES:%=-I$(PROJROOT)/%) 
