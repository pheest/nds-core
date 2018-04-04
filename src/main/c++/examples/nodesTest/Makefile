CXX =g++

STD =-std=c++0x
INCLUDES =-I../../include -I/opt/codac/include
DEFS =-DEPICS -DNDS3_DLL
CXXFLAGS =$(STD) -Wall -Wextra -pedantic -fPIC -pthread $(DEFS) -g
LDFLAGS =-L../../../../lib -L/opt/codac/lib -g


CXXFLAGS +=$(INCLUDES)
TARGETDIR =../../../../lib
# Flags passed to gcc during linking
LINK = -shared -Wl,-as-needed
 
# List of targets 
TARGET = $(patsubst %.cpp,$(TARGETDIR)/libnds3-%.so,$(wildcard *.cpp))
 
# Additional linker libraries
LIBS =-lnds3
 
all: $(patsubst %.cpp, %.so,$(wildcard *.cpp))

%.so: %.cpp 
	mkdir -p $(TARGETDIR)
	$(CXX) $< $(CXXFLAGS) $(LINK) $(LDFLAGS) -o $(TARGETDIR)/libnds3-$@ $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) 
 
