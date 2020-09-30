## 〽️ C++ sorting algorithms
Using two different sorting algorithms and two different paralellization strategies, this program sorts a text file of unique integers and outputs the sorted list. The text file will be an unsorted data structure [Standard Template Library (STL) vector]. This is to emulate the performance of the UNIX `sort-n` command.

#### Sorting Algorithms:
  1. **Quicksort** - picks element as pivot and partition exchanging
  2. **Mergesort** - divides into two halfs, then merges sorted halfs
  3. **Bucketsort** - sorts array into different bins, then sorts the bins

#### 🗜️ Added functionality:
  1. Fork/join parellelism - implementation of fork, join, and barriers
  2. Locks - synchronization mechanism for enforcing limits of resource access; also known as mutex (mutual exlcusion). Used in conjunction with multiple threading a process.

---
### 🗄️ Code Organization

### 📁 Files
  1. `main.cpp`
  1. `bucketsort.cpp`
  2. `mergesort.cpp`
  3. `Makefile`

#### 💾 Additional files
  1. randomNumberGen.cpp - creates a text file with a specified number of random numbers from [ 1 - number size ]. Number size is specified within cpp file itself.
  2. mergesort and quicksort cpp + header files. Not used, instead directly incorporated to main.cpp.

### Compiling
  Note: If zipped, unzip file.
  1.


### Execution
`mysort [--name] [source.txt] [-o out.txt] [-t NUM THREADS] [--alg=<fjmerge,fjquick,lkbucket>]`
  1.

### 🐜 Surviving Bugs
  - None witnessed
