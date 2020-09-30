### C++ sorting algorithms
This program sorts a text file of unique integers and outputs the sorted list. The text file will be an unsorted data structure [Standard Template Library (STL) vector]. This is to emulate the performance of the UNIX `sort-n` command.

Sorting Algorithms (through divide and conquer):
  1. **Quicksort** - picks element as pivot and partition exchanging
  2. **Mergesort** - divides into two halfs, then merges sorted halfs
  3. **Bucketsort** - sorts array into different bins, then sorts the bins

#### Makefile
1. `make` creates a mysort exe file
2. `make clean` removes any files created by make

#### Additional files
1. randomNumberGen.cpp - creates a text file with a specified number of random numbers from [ 1 - number size ]. Number size is specified within cpp file itself.
2. mergesort and quicksort cpp + header files. Not used, instead directly incorporated to main.cpp.

