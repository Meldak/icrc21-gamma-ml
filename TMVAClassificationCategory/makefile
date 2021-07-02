CC=cc
CXX=c++

#CXXFLAGS=-Wall -O2 #-march=native #-std=c++11

ROOTFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs)

all: main

#debug: all
#debug: CXXFLAGS += -DDEBUG -g

root-ls: TMVAClassificationCategory.C   
	$(CXX) $(ROOTFLAGS) -o $@ $^ $(ROOTLIBS)

clean:
	$(RM) -rf TMVAClassificationCategory