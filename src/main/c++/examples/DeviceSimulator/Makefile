CXX = g++
 
CXXFLAGS = -std=c++0x -g -Wall -Wextra -pedantic -fPIC -pthread -DNDS3_DLL -I../../include -I/opt/codac/include -I/usr/include/boost/regex
LDFLAGS = -L../../../../lib -L/opt/codac/lib

TARGETDIR = ../../../../lib
LINK = -shared -fPIC -Wl,-as-needed

# Name of the device library
TARGET = libnds3-dev-devicesimulator.so

# Additional linker libraries
LIBS = -lnds3  -lboost_regex
 
# Source code files used in this project
SRCS = DeviceSimulator.cpp
 
OBJS = $(SRCS:.cpp=.o)

all: $(TARGETDIR)/$(TARGET)

# Rules for building
$(TARGETDIR)/$(TARGET): $(OBJS)
	mkdir -p $(TARGETDIR)
	$(CXX) $(LINK) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

