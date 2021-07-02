CCC = g++       # compiler
DICT = rootcint # root dictionary-generator
AR = ar         # archiver
LD = g++        # linker

#--------------------------------------------------------------------------------------------

export COAST_DIR=/home/software/corsika/corsika-77410

#--------------------------------------------------------------------------------------------

LDFLAGS   = ${CXXFLAGS} -fPIC -ggdb3
LDFLAGS  += $(shell root-config --libs)
LDFLAGS  += -lTMVA
LDFLAGS  += -lTMVAGui
LDFLAGS  += -L$(COAST_DIR)/lib -L$(COAST_DIR)/lib/unknown
LDFLAGS  += -lCorsikaFileIO
LDFLAGS  += -lCorsikaIntern
LDFLAGS  += -lCorsikaToROOT
LDFLAGS  += -lCorsikaROOT

CPPFLAGS  = ${CXXFLAGS} -c -fPIC -ggdb3 
CPPFLAGS += -I$(COAST_DIR)/include
CPPFLAGS += $(shell root-config --cflags)

SRCFILES  = $(wildcard *.cc)
OBJECTS   = $(patsubst %.cc, %.o, ${SCRFILES})

EXE       = CortoRoot
EXE 	 += corsika2root

all: ${EXE}

#-----------------------------------------------------------------------------------------

default: CortoRoot

CortoRoot:  CortoRoot.o
		${CXX} $^ ${LDFLAGS} -o $@

corsika2root: corsika2root.o
		${CXX} $^ ${LDFLAGS} -o $@

test: CortoRoot.o
		${CXX} $^ ${LDFLAGS} -o $@

clean:
	@rm -f *.o *.so *.png
