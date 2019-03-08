#+======================================================================
# $HeadURL$
# $Id$
#
# Project       : CODAC Core System
#
# Description   : Makefile.
#
# Authors       : James Blair (National Instruments)
#                 Mike Hoogendyk (National Instruments)
#
# Copyright (c) : 2013-2017 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file ITER-LICENSE.TXT located in the top level directory
# of the distribution package.
#
#-======================================================================

NDSLIBDIR:=$(realpath ../../../../target/lib)
# Path from which the device source file will be obtained.
EXAMPLESDIR:=$(realpath ../../../main/c++/examples/nodesTest)
EXAMPLESHEADERS:=$(wildcard $(EXAMPLESDIR)/*.h))
# Necessary for make to find the source files in a different directory.
VPATH =$(EXAMPLESDIR)


INCLUDES :=-I../../../main/c++/include -I$(EXAMPLESDIR) -I../include
LIBPATH :=-L../../../lib -Wl,-rpath=$(NDSLIBDIR)


#############################################################################


CXXFLAGS +=$(INCLUDES) --coverage -MMD -MP -g -Wall -std=c++11
LDFLAGS +=$(LIBPATH) -lpthread -lnds3 -ldl -lrt -lgtest

TESTS_OBJS := $(patsubst $(EXAMPLESDIR)/%.cpp, %.o, $(wildcard $(EXAMPLESDIR)/*.cpp))
TESTS_OBJS += $(patsubst %.cpp, %.o, $(wildcard *.cpp))

TEST =nds3core-unit-tests

all: $(TEST)

$(TEST): $(TESTS_OBJS) 
	$(CXX) $^ --coverage -o $@ $(LDFLAGS)


clean:
	@$(RM) $(TESTS) *.o *.d

