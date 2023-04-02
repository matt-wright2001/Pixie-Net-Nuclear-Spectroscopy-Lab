CC=gcc
CXX=g++
CXXLD=g++
CCLD=gcc

CXXFLAGS= -g -Wall -I -I$  -pthread -fPIC -std=c++11 -DPIC
CCFLAGS=$(CXXFLAGS)

ROOTCFLAGS   := $(shell ${ROOTSYS}/bin/root-config --cflags)
ROOTGLIBS    := $(shell ${ROOTSYS}/bin/root-config --glibs)
ROOTLDFLAGS  := $(shell ${ROOTSYS}/bin/root-config --ldflags)
CXXFLAGS+=$(ROOTCFLAGS)

INCLUDES:= rootConvertPixieNet.h 

OBJECTS:= rootConvertPixieNet.o

#LIBS  = -lm $(ROOTGLIBS) -Wl,-rpath,/user/e14057/ddasdumper -L/user/e14057/ddasdumper -lddaschannel -Wl,-rpath,/user/e14057/prokop/Utilities -L/user/e14057/prokop/Utilities -lPixie16Utilities

#LIBS  = -lm $(ROOTGLIBS) -Wl,-rpath,/user/e14057/ddasdumper -L/user/e14057/ddasdumper -lddaschannel

#LIBS:= -lm $(ROOTGLIBS) -Wl,-rpath,/usr/opt/ddas/3.0/lib -L/usr/opt/ddas/3.0/lib -lddaschannel -Wl,-rpath,/home/cridaq/Utilities_root6  -L/home/cridaq/Utilities_root6 -lPixie16Utilities
 
LIBS:= -lm $(ROOTGLIBS)

SRCS:= rootConvertPixieNet.cpp

all: rootConvertPixieNet

rootConvertPixieNet:  $(SRCS) $(OBJECTS) $(INCLUDES) 
	$(CXX) -o rootConvertPixieNet $(OBJECTS) $(CXXFLAGS) $(ROOTGLIBS) $(LIBS)

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -c $< 
%.o: %.cxx 
	$(CXX) $(CXXFLAGS) -c $< 
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< 

clean:
	rm -f *.o rootConvertPixieNet
# DO NOT DELETE

print_env:
	@echo $(ROOTSYS)
	@echo $(ROOTCFLAGS)
	@echo $(ROOTGLIBS)
	@echo $(ROOTLDFLAGS)

