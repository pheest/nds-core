CXX =g++

STD =-std=c++11
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

#LIBRARIES=nds3 
#LIBNAME=nds3-DeviceDBL
 
all: $(patsubst %.cpp, %.so,$(wildcard *.cpp))

%.so: %.cpp 
	mkdir -p $(TARGETDIR)
	$(CXX) $< $(CXXFLAGS) $(LINK) $(LDFLAGS) -o $(TARGETDIR)/libnds3-$@ $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) 

listdeps:
	@$(foreach lib,$(patsubst %.cpp,%,$(wildcard *.cpp)),echo -e "Dependencies of 'nds3-$(lib)' library:\n* syslib: nds3\n* libdir: \n";)
#           @for lib in $(patsubst %.cpp,%,$(wildcard *.cpp)); do \
#           echo "Dependencies of '${lib}' library:"; \
#           echo "* syslib: "; \
#           echo "* libdir: "; done
             

 
