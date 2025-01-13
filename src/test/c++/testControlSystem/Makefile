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
# Copyright (c) : 2013-2024 ITER Organization,
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

LIBNAME=nds3-testcontrolsystem

TARGET=../../../../target

LIBRARIES=nds3

LIBRARY_DIRS=$(TARGET)/lib
INCLUDE_DIRS=./include ../include $(TARGET)/main/c++/include

LIBRARY_DIR=$(TARGET)/lib
SOURCES_DIR=.
OBJECT_DIR=$(SOURCES_DIR)/.obj

SHAREDLIBRARY=$(LIBRARY_DIR)/lib$(LIBNAME).so
STATICLIBRARY=$(LIBRARY_DIR)/lib$(LIBNAME).a
INCLUDES=$(foreach inc,$(INCLUDE_DIRS),-I$(inc))
LDPATHS=$(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) 
LDLIBS=$(foreach libs,$(LIBRARIES),-l$(libs))
SOURCES=$(wildcard *.cpp)
OBJECTS=$(addprefix $(OBJECT_DIR)/,$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(notdir $(SOURCES)))))

C=gcc
CC=g++
CFLAGS=-c -Wall -Wextra -Wpedantic -Wshadow -fPIC
CCFLAGS=-c -Wall -Wextra -Wpedantic -Wshadow -fPIC -std=c++11
LDFLAGS= -shared

ifeq ($(COVERAGE),true)
	CFLAGS+= -O0 -g --coverage
	CCFLAGS+= -O0 -g --coverage
	LDFLAGS+= --coverage
else
	CFLAGS+= -O3
	CCFLAGS+= -O3
endif

VPATH=$(SOURCES_DIR)


.PHONY: all clean run 

all: $(SOURCES) $(SHAREDLIBRARY) $(STATICLIBRARY)

clean:
	rm -rf "$(SHAREDLIBRARY)" "$(STATICLIBRARY)" "$(OBJECT_DIR)" "$(HTARGETS)"

run: $(SOURCES) $(SHAREDLIBRARY) 
	$(SHAREDLIBRARY)

$(SHAREDLIBRARY): $(OBJECTS)
	mkdir -p $(LIBRARY_DIR)
	$(CC) $(LDFLAGS) $(LDPATHS) $(OBJECTS) -o $(SHAREDLIBRARY) $(LDLIBS)

$(STATICLIBRARY): $(OBJECTS)
	mkdir -p $(LIBRARY_DIR)
	$(AR) rcs $@ $^
	
$(OBJECT_DIR)/%.o: %.cpp
	mkdir -p $(OBJECT_DIR)
	$(CC) $(CCFLAGS) $(INCLUDES) $< -o $@

$(OBJECT_DIR)/%.o: %.c
	mkdir -p $(OBJECT_DIR)
	$(C) $(CFLAGS) $(INCLUDES) $< -o $@
