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

EXE       = main
#EXE       = energySpectra
# EXE      += angularDistribution
# EXE      += groundMomenta
# EXE      += footprint
# EXE      += dummySim
# EXE      += longReader
all: ${EXE}

#-----------------------------------------------------------------------------------------

default: main

main:  main.o
		${CXX} $^ ${LDFLAGS} -o $@

test: main.o
		${CXX} $^ ${LDFLAGS} -o $@

# energySpectra:  energySpectra.o
# 		${CXX} $^ ${LDFLAGS} -o $@

# angularDistribution: angularDistribution.o
# 		${CXX} $^ ${LDFLAGS} -o $@

# groundMomenta:   groundMomenta.o
# 		${CXX} $^ ${LDFLAGS} -o $@

# footprint:      footprint.o
# 		${CXX} $^ ${LDFLAGS} -o $@

# dummySim:       dummySim.o dummyExperiment.o
# 		${CXX} $^ ${LDFLAGS} -o $@

# longReader:     longReader.o
# 		${CXX} $^ ${LDFLAGS} -o $@

clean:
	@rm -f *.o *.so *.png
