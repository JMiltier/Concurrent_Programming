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
  1. `main.c`
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
  1. `mysort --name`: prints name to console.
  2. `source.txt`: unsorted txt file of numbers, with each number on a new line
  3. `-o out.txt`: sorted txt file of numebrs, in which the program outputs/writes to
  4. `-t NUM_THREADS`: specify how many threads to use during execution (including master thread)
  5. `--alg=<fjmerge, lkbucket>`: specify which algorithm to use.
      - fjmerge: merge sort using fork/join
      - lkbucket: bucket sort using locks
  - **Additional outputs**: time of execution in nanoseconds

### 🐜 Surviving Bugs
  - None witnessed
