# Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
#
# tests/ut/Makefile - builds and runs unit tests
#
#Licensed under MIT License, see full text in LICENSE
#or visit page https://opensource.org/license/mit/

include ../common/make.mk

STD = c++20
BDIR = $(BUILDDIR:%=%/$(STD))
PROJROOT := $(abspath $(dir $(abspath $(firstword $(MAKEFILE_LIST))))/../../)
SRCS := $(shell ls -1 *.cpp)
OBJS := $(SRCS:%.cpp=$(BDIR)/%.o)
EXE  = $(BDIR:%=%/)ut
BOOST_UT = $(PROJROOT)/tests/common/boost/ut.hpp
BOOST_URL = https://raw.githubusercontent.com/boost-ext/ut/refs/heads/master/include/boost/ut.hpp

all: build run

build: $(EXE)

run: $(EXE)
	 ./$(EXE)

$(EXE): $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(BDIR)/%.o: %.cpp | $(BDIR) $(BOOST_UT)
	$(info $(STD) $^)
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BDIR):
	@mkdir -p $@

$(BOOST_UT):
	@mkdir -p $(dir $@)
	@wget $(BOOST_URL) -P $(dir $@)

clean:
	@$(BDIR:%=rm -f %/*) 

clean-all:
	@$(BUILDDIR:%=rm -rf %/*) 
