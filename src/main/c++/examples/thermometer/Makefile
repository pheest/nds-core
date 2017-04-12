CXX = g++

CXXFLAGS = -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -DNDS3_DLL -I../../include -I/opt/codac/include
LDFLAGS = -L../../../../lib -L/opt/codac/lib

TARGETDIR = ../../../../lib
# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed

# Name of our device library
TARGET = libnds3-dev-thermometer.so
 
# We specify that we depend on NDS3
LIBS = -lnds3
 
# We assume that all the .cpp files are in the src folder
SRCS = thermometer.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGETDIR)/$(TARGET)

# Rules for building
$(TARGETDIR)/$(TARGET): $(OBJS)
	mkdir -p $(TARGETDIR)
	$(CXX) $(LINK) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
