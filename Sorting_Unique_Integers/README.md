### C++ sorting algorithms
This program sorts a text file of unique integers and outputs the sorted list. The text file will be an unsorted data structure [Standard Template Library (STL) vector]. This is to emulate the performance of the UNIX `sort-n` command.

Sorting Algorithms (through divide and conquer):
  1. **Quicksort** - picks element as pivot and partition exchanging
  2. **Mergesort** - divides into two halfs, then merges sorted halfs

#### Makefile
<<<<<<< HEAD
1. `make` creates a mysort exe file
  Functionality:s
  - `mysort --name` displays first name and last name
  - `mysort <input file> -o <output file> --alg=<merge,quick>` takes an input file, writes to an output file; the input file is sorted based on specified merge or quick, and written to the output file
  - no other mysort features available; no current fact checking
=======
1. `make` creates a mysort exe file  
    Functionality:
    - `mysort --name` displays first name and last name
    - `mysort <input file> -o <output file> --alg=<merge,quick>` takes an input file, writes to an output file; the input file is sorted based on specified merge or quick, and written to the output file
    - no other mysort features available; no current fact checking
>>>>>>> f60b6ec690dbfdfb093997a491db7a32776b78fb
2. `make clean` removes any files created by make

#### Additional files
1. randomNumberGen.cpp - creates a text file with a specified number of random numbers from [ 1 - number size ]. Number size is specified within cpp file itself.
2. mergesort and quicksort cpp + header files. Not used, instead directly incorporated to main.cpp.

