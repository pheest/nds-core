CXX =g++

SOURCES =$(wildcard ndsDrv/*.cpp)
OBJECTS =$(patsubst ndsDrv/%.cpp, ndsDrv/%.o, $(SOURCES))
HEADERS =$(wildcard ndsDrv/*.h)

STD =-std=c++0x
INCLUDES =-I../../include -I/opt/codac/include
DEFS =-DEPICS -DNDS3_DLL
DBG =-g
CXXFLAGS =$(STD) -Wall -Wextra -pedantic $(DBG) $(INCLUDES)

NOLINK =-c -fPIC -pthread
LINK =-shared -Wl,-as-needed

# Additional linker libraries
LIBS =-lnds3
LDFLAGS =-L../../../../lib -L/opt/codac/lib


TARGETDIR =$(realpath ../../../../lib)

#STATICLIB =symDev/symulating-device.a
STATICLIB =
DYNLIB =$(TARGETDIR)/libnds3-oscilloscope.so
DEPENDENCIES =$(DYNLIB)
#DEPENDENCIES +=$(STATICLIB)


all: $(DEPENDENCIES)

$(TARGETDIR)/libnds3-oscilloscope.so: $(OBJECTS)
	mkdir -p $(TARGETDIR)
	$(CXX) $^ $(CXXFLAGS) $(LINK) $(LIBS) $(STATICLIB) $(LDFLAGS) -o $@

$(STATICLIB): symDev/%.cpp
	ar rc $@ $^

ndsDrv/%.o: ndsDrv/%.cpp $(HEADERS)
	$(CXX) $< $(CXXFLAGS) $(DEFS) $(NOLINK) -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET)
