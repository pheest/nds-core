CXX = g++
 
CXXFLAGS = -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -DNDS3_DLL -I../../include -I/opt/codac/include -g -DEPICS
LDFLAGS = -L../../../../lib -L/opt/codac/lib -g


INCLUDEDIR=-I../include
CXXFLAGS += $(INCLUDEDIR)
TARGETDIR = ../../../../lib
# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed
 
# List of targets 
TARGET = $(patsubst %.cpp,$(TARGETDIR)/libnds3-%.so,$(wildcard *.cpp))
 
# Additional linker libraries
LIBS = -lnds3
 
all: $(patsubst %.cpp, %.so,$(wildcard *.cpp))

%.so: %.cpp 
	mkdir -p $(TARGETDIR)
	$(CXX) $< $(CXXFLAGS) $(LINK) $(LDFLAGS) -o $(TARGETDIR)/libnds3-$@  $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) 
 
