# Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
#
# tests/ft/Makefile - builds and runs functional tests
#
#Licensed under MIT License, see full text in LICENSE
#or visit page https://opensource.org/license/mit/

include ../common/make.mk
SHELL=/usr/bin/bash

STD = c++20
BDIR = $(BUILDDIR)
PROJROOT := $(abspath $(dir $(abspath $(firstword $(MAKEFILE_LIST))))/../../)/
SRCS := $(shell ls -1 *.cpp)
OBJS := $(SRCS:%.cpp=$(BDIR)/%.o)
EXE  = $(BDIR:%=%/)ft
FTLS  = $(BDIR:%=%/)ftls
LIBS = :libusb-1.0.a udev
LIBUSB_DIR = $(PROJROOT)ext/libusb
USBUTILS_DIR = $(PROJROOT)ext/usbutils
# build/lsusb.p
USBUTILS_OBJS = lsusb.c.o lsusb-t.c.o desc-dump.c.o usbmisc.c.o sysfs.c.o names.c.o desc-defs.c.o
LIBDIRS = $(LIBUSB_DIR:%=%/libusb/.libs)
INCLUDES += ext/libusb ext/libusb/libusb
TESTS := $(shell cd data && ls -1 *:*)
RUNS  := $(shell cd data && ls -1 *.run)

all: $(EXE) $(FTLS)

run: $(EXE)
	@$(foreach t, $(TESTS), diff -y --suppress-common-lines data/$t <($(EXE) -v -s $t) \
	  && echo "ft -v -s $t	PASS" || echo "ft -v -s $t	FAIL";)
	@$(foreach t, $(RUNS), diff -y --suppress-common-lines data/$(t:%.run=%.master) <($(SHELL) data/$t) \
	  && echo "$t	PASS" || echo "$t	FAIL";)

masters: $(RUNS:%.run=data/%.master) | $(FTLS)

$(LIBUSB_DIR:%=%/libusb/.libs):
	@cd $(LIBUSB_DIR) && ./autogen.sh && make install

$(USBUTILS_DIR)/build/lsusb.p:
	@cd $(USBUTILS_DIR) && ./autogen.sh build

$(EXE): $(OBJS) $(USBUTILS_OBJS:%=$(USBUTILS_DIR)/build/lsusb.p/%) | $(USBUTILS_DIR)/build/lsusb.p $(LIBUSB_DIR:%=%/libusb/.libs)
	$(info link $@)
	@$(CXX) $(CXXFLAGS) $(LIBDIRS:%=-L%) $^ $(LIBS:%=-l%) -o $@

$(FTLS): $(BDIR)/ftls.o $(OBJS) | $(LIBUSB_DIR:%=%/libusb/.libs)
	$(info cxx  $@)
	@$(CXX) $(CXXFLAGS) $(LIBDIRS:%=-L%) $^ $(LIBS:%=-l%) -o $@

$(BDIR)/%.o: %.cxx | $(BDIR)
	$(info cxx  $^)
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BDIR)/%.o: %.cpp | $(BDIR)
	$(info cxx  $^)
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BDIR):
	@mkdir -p $@

data/%.master: data/%.run
	$(SHELL) $^ > $@

clean:
	@$(BDIR:%=rm -rf %/*) 

