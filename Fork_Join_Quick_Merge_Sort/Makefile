# --------------------------------------------------------
# Makefile - versitility to use again for C or C++ files
# 'make'        builds executable targets
# 'make clean'  removes all .o and executable files
# --------------------------------------------------------

# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g			adds debugging information to the executable file (dSYM files)
#  -Wall	turns on most, but not all, compiler warnings
#  -w			inhibits all warning messages
CFLAGS  = -w

MYPROGRAM=mysort
SOURCE=main.cpp
name= "Josh Miltier"

$(MYPROGRAM): clean $(SOURCE)
	$(CC) $(CFGLAGS) $(SOURCE) -o $(MYPROGRAM)

numGen: randomNumberGen.cpp
	$(CC) $(CFGLAGS) randomNumberGen.cpp -o numGen

clean:
	rm -rf $(MYPROGRAM) numGen