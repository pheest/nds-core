
#libraries
LIBRARIES= nds3
LIBRARY_DIRS=../../../../lib
LDLIBS=-L$(CODAC_ROOT)/lib $(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) $(foreach libs,$(LIBRARIES),-l$(libs))

#includes
INCLUDE_DIRS=. ../../include/
INCLUDES=$(foreach inc,$(INCLUDE_DIRS),-I$(inc))

#Sources
SOURCE_DIR=.
SOURCES:=$(SOURCE_DIR)/$(wildcard *.cpp)

TARGET=../../../..
LIBRARY_DIR=$(TARGET)/lib
OBJECT_DIR=$(SOURCE_DIR)/.obj
SHAREDLIBRARIES=$(addprefix $(LIBRARY_DIR)/,$(patsubst %.cpp,libnds3-%.so,$(notdir $(SOURCES))))

#Objects
OBJECTS:=$(addprefix $(OBJECT_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))

CXX=g++

STD=-std=c++11
#CFLAGS=-c -Wall -fPIC 
CXXFLAGS=-c $(STD) -Wall -Wextra -pedantic -fPIC -pthread  -fvisibility-inlines-hidden $(DEFS) 
LDFLAGS= -shared 

ifeq ($(COVERAGE),true)
	#CFLAGS+= -O0 -g --coverage
	CXXFLAGS+= -O0 -g --coverage
	LDFLAGS+= --coverage
endif

all: $(SHAREDLIBRARIES)

clean:
	rm -rf "$(SHAREDLIBRARIES)" "$(OBJECT_DIR)" "$(HTARGETS)"

$(LIBRARY_DIR)/libnds3-%.so: $(OBJECT_DIR)/%.o 
	@mkdir -p $(LIBRARY_DIR)
	$(CXX) $(LDFLAGS) $(LDLIBS) $< -o $@
	
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp 
	@mkdir -p $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@
