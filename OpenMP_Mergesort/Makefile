# --------------------------------------------------------
# Makefile - versitility to use again for C or C++ files
# 'make'         builds executable targets
# 'make numGen'  creates random number generator object
# 'make clean'   removes all .o and executable files
# --------------------------------------------------------

# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g			adds debugging information to the executable file (dSYM files)
#  -Wall	turns on most, but not all, compiler warnings
#  -w			inhibits all warning messages
CFLAGS = -w -openmp

# GCC standard support for different C++ dialects (98, 11, 14, 17, or 2a)
STD = -std=c++14

SORTPRGM=mysort
SORTSRC=mysort.cpp
NUMGENPRGM=numGen
NUMGENSRC=randomNumberGen.cpp

all: $(SORTPRGM)
.PHONY: clean

$(SORTPRGM): $(SORTSRC)
	$(CC) $(STD) $(CFLAGS) $(SORTSRC) -o $(SORTPRGM)

$(NUMGENPRGM): $(NUMGENSRC)
	$(CC) $(STD) $(NUMGENSRC) -o $(NUMGENPRGM)

clean:
	rm -rf $(SORTPRGM) $(NUMGENPRGM)