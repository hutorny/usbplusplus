# Copyright (C) 2025 Eugene Hutorny <eugene@hutorny.in.ua>
#
# tests/ct/Makefile - builds compile tests
#
#Licensed under MIT License, see full text in LICENSE
#or visit page https://opensource.org/license/mit/

include ../common/make.mk

STD = c++14
BDIR = $(BUILDDIR:%=%/$(STD))
PROJROOT := $(abspath $(dir $(abspath $(firstword $(MAKEFILE_LIST))))/../../)/
SRCS := $(shell ls -1 *.cpp)
OBJS := $(SRCS:%.cpp=$(BDIR)/%.o) 

ifdef STDS
all:
	@$(foreach s,$(STDS),$(MAKE) --no-print-directory STDS= STD=$(s);)
else
all: $(OBJS) 
endif

$(BDIR)/%.o: %.cpp | $(BDIR)
	$(info $(STD) $^)
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

$(BDIR):
	@mkdir -p $@

clean:
	@$(BDIR:%=rm -f %/*) 

clean-all:
	@$(BUILDDIR:%=rm -rf %/*) 
