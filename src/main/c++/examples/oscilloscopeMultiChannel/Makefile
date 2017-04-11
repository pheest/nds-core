CXX = g++
 
CXXFLAGS = -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -DNDS3_DLL -I../../include
LDFLAGS = -L../../../../lib

TARGETDIR = ../../../lib
# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed
 
# Name of the nds library
TARGET = libnds3-dev-oscilloscopemultichannel.so
 
# Additional linker libraries
LIBS = -lnds3
 
# Source code files used in this project
SRCS = oscilloscopeMultiChannel.cpp
 
OBJS = $(SRCS:.cpp=.o)
# Rules for building
$(TARGETDIR)/$(TARGET): $(OBJS)
	$(CXX) $(LINK) $(LDFLAGS) -o $@ $^ $(LIBS)
 
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
