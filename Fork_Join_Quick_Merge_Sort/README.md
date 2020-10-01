## 〽️ C++ sorting algorithms
Using two different sorting algorithms and two different paralellization strategies(fork/join and locks), this program sorts a text file of unique integers and outputs the sorted list to a text file. The text file will be an unsorted data structure [Standard Template Library (STL) vector]. This is to emulate the performance of the UNIX `sort-n` command.

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
  1. `randomNumberGen.cpp`- creates a text file with a specified number of random numbers from [ 1 - number size ]. Default number is 10,000 unless specified when ran (as second argument).
  2. mergesort and quicksort cpp + header files. Not used, instead directly incorporated to main.cpp.

### Compiling
  Note: If zipped, unzip file.
  1. From root directory, run `make` from the terminal. This will generate a program called *mysort*
  2. Additionally, if wanting to create a text file (*source.txt*) of randomly generated numbers (repeatable), run `make numGen` from the terminal. Then, `./numGen <amount of numbers to generate>`. If a second argument isn't passed in (`./numGen`), the program will generate 10,000 numbers by default.
  3. Next, follow execution syntax below for parallel algoritm sorting.

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
  - Input/output files are loaded/saved as is. Does not check for txt file types.
  - Numbers within input txt file must be integers (not checked) for the program to work.
