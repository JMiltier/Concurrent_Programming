# --------------------------------------------------------
# Makefile - versitility to use again for C or C++ files
# 'make'         builds executable targets marked from 'all:'
# 'make mysort'  creates executable object for sort algos
# 'make counter' creates executable object for counter
# 'make numGen'  creates random number generator object
# 'make clean'   removes all .o and executable files
# --------------------------------------------------------

# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g			adds debugging information to the executable file (dSYM files)
#  -Wall	turns on most, but not all, compiler warnings
#  -w			inhibits all warning messages
CFLAGS = -w

# GCC standard support for different C++ dialects (98, 11, 14, 17, or 2a)
STD = -std=c++11

# variables
SORTPRGM=mysort
SORTSRC=mysort.cpp
CNTRPRGM=counter
CNTRSRC=counter.cpp
NUMGENPRGM=numGen
NUMGENSRC=randomNumberGen.cpp

all: clean $(CNTRPRGM) $(SORTPRGM)

# counter
$(CNTRPRGM): $(CNTRSRC)
	$(CC) $(STD) $(CFLAGS) $(CNTRSRC) -o $(CNTRPRGM) -lpthread

# mysort
$(SORTPRGM): $(SORTSRC)
	$(CC) $(STD) $(CFLAGS) $(SORTSRC) -o $(SORTPRGM) -lpthread

# random numbers generation
$(NUMGENPRGM): $(NUMGENSRC)
	$(CC) $(CFLAGS) $(NUMGENSRC) -o $(NUMGENPRGM)

# clean all objects
clean:
	rm -rf $(SORTPRGM) $(CNTRPRGM) $(NUMGENPRGM)