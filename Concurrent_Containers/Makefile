# --------------------------------------------------------
# Makefile - versitility to use again for C or C++ files
# 'make'             cleans and builds executable targets marked from 'all:'
# 'make containers'  creates executable object for sort algos
# 'make numGen'      creates random number generator object
# 'make clean'       removes all .o and executable files
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
CNTNRPRGM=containers
CNTNRSRC=containers.cpp
NUMGENPRGM=numGen
NUMGENSRC=randomNumberGen.cpp

all: clean $(CNTNRPRGM)

# containers
$(CNTNRPRGM): $(CNTNRSRC)
	$(CC) $(STD) $(CFLAGS) $(CNTNRSRC) -o $(CNTNRPRGM) -pthread

# random numbers generation
$(NUMGENPRGM): $(NUMGENSRC)
	$(CC) $(CFLAGS) $(NUMGENSRC) -o $(NUMGENPRGM)

# clean all objects
clean:
	rm -rf $(CNTNRPRGM) $(NUMGENPRGM)