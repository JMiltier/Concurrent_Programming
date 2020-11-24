# 🔒 C++ locking algorithms
Using two different sorting algorithms and two different paralellization strategies(fork/join and locks), this program sorts a (specified) *.txt*  file of unique integers and outputs the sorted list to a different (specified) *.txt* file. The input *.txt* file will be an unsorted data structure [Standard Template Library (STL) vector]. This is to emulate the performance of the UNIX `sort-n` command.

#### Barrier Algorithms:
  1. **Sense-reversal barrier** - Each sequential barrier uses an opposite value of the previous barrier's value as a pass/stop state. By not using the same value, this barrier algorithm solves for potential deadlocking issues. For example, if barrier 1 uses the value 0 to stop the threads, barrier 2 will use 1 to stop, barrier 3 will use 0, barrier 4 will use 1, and so forth. In the algorithm, it simply flips (0/1) the value each time. 
  2. **Pthread barrier** - Implementation of pthread locks is setup from initializing how many threads to expect at the barrier. Once all threads have arrived, the barrier call will *unlock* and the threads will continue until either finished, or another barrier is set. Once the threads are complete, they will join to the main thread as a sense of notification that the worker threads are complete (and then dead).

#### Locking Algorithms:
  1. **Test-and-set (TAS) lock** - using a single atomic value, TAS functionality tests a lock (true/false; obtain when false), and then sets lock. In the algorithm, a constant spinning loop tests against the lock to see when it's set to false (0). And once 0, it obtains the lock by setting the atomic value to 1. One issue with TAS is it's unfairness with not guaranteeing FIFO ordering for threading competing against the lock. This algorithm is easy to implement, but horrible for scaling. 
  2. **Test-and-test-and-set (TTAS) lock** - more elaborate locking based on a modified version of TAS. In additional to test the lock, it also continuously checks the memory to see if the lock has been obtained. Once free, it attempts to take the lock by using test and set. So the additional 'test' comes from testing the memory of the lock, before trying to test and set it. 
  3. **Ticket lock** - a queue like algorithm that uses two atomic counters, one which designates who is up, and who is next. Though this could lead to starvation of some threads, it allows for the next thread (among all) to know it's got CPU access next. This is also a spinlock, where the next in the queue waits until it's equal to the value now available. Both of these counters will increment as the locks/unlocks occur. 
  4. **MCS bock** - <not implemented> 
  5. **Pthread lock** - using mutex locks and an atomic shared variable 

#### Sorting List Structure
  - Migrated from an *array*, and switched to using a *vector*. Some differences (and ultimate reasoning for decision) are outlined below.  

                | Array   | Vector  | Additional Notes |
  |:----        | :---:   | :----:  |  :--
  |Synchronized | No      | Yes     |   1
  |Implement | Statically or Dynamically | Dynamic arrays with list interface | 2
  |Size         | Fixed | Dynamic |   3 
  |Deallocation | Explicitly  | Automatically | 4
  |Size Determination | undetermined | O(1) time | 5
  |Reallocation | Explicitly  | Implicitly  | 6
  |Returned through func? | No | Yes 
  |Copied Directly? | No  | Yes
  |Assigned Directly? | No | Yes


  1. Only one thread at a time can access the code with vectors, while arrays allow for multiple threads to work on it at the same time. This is turn causes thread waiting for vectors.
  2. 
  3. Size: The size of an array is fixed, and expensive to reallocate. Vectors, on the other hand, are resizable since they are allocated on heap memory. 

#### 🗜️ Added functionality:
  1. Fork/join parellelism - implementation of fork, join, and barriers
  2. Locks - synchronization mechanism for enforcing limits of resource access; also known as mutex (mutual exlcusion). Used in conjunction with multiple threading a process.

---
## 🗄️ Code Organization

### 📁 Files
  1. `counter.cpp` - program to incorperate bars and locks
  2. `Makefile` - create executable objects
  3. `bucketsort.h` - header file for bucket sort using locks
  4. `counter.h` - header file for merge sort using fork/join
  4. `cnt_arg_parser.h` - 
  5. `arg_parser.h` - error handling and parsing for program's input options
  6. `README.pdf` - write-up for project

### 💾 Additional files
  1. `randomNumberGen.cpp`- creates a text file with a specified number of random numbers from [ 1 - number size ]. Default number is 10,000 unless specified when ran (as second argument).
  2. `pthread_add.h` - threading if compiling on macOS (will still run as normal for linux systems)

### Compiling
  Note: If zipped, first unzip file before proceeding.
  1. From root directory, run `make` from the terminal. This will generate a program called *counter*
  2. Next, follow execution syntax below for parallel algoritm sorting.
  3. `Make clean` will remove all files generated by `make`.

## Execution
#### Program option/input parameters for `counter`
`counter [--name]`
  - `counter --name`: prints name to console.
**OR**
`counter [-t NUM THREADS] [-i=NUM_ITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>] [-o out.txt]`
  1. `-t NUM_THREADS`: specify how many threads to use during execution (including master thread)
  2. `-i=NUM_ITERATIONS`:
  3. `--bar=<sense,pthread>`:
  4. `--lock=<tas,ttas,ticket,mcs,pthread>`:
  5. `-o out.txt`: sorted txt file of numebrs, in which the program outputs/writes to
  - **Additional outputs**: time of execution in nanoseconds

#### Program option/input parameters for `mysort`
  `mysort [--name]`
    - `mysort --name`: prints name to console.
  **OR**
  `mysort [source.txt] [-o out.txt] [-t NUM THREADS] [--alg=<bucket>] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>]`
  1. `source.txt`: input file for unsorted and random list of numbers, each on a new line
  2. `-o out.txt`: output fil for sorted list of numbers (from 1.), each on a new line
  3. `-t NUM_THREADS`: specify how many threads to use during execution (including master thread)
  4. `--alg=<bucket>`: sorting algorithm
  5. `--bar=<sense,pthread>`: type of bar
  6. `--lock=<tas,ttas,ticket,mcs,pthread>`: type of lock 

## Program stats average, based on 10 repeated runs
### Barriers
#### For L1 cache hit rate
`perf stat -e L1-dcache-loads -e L1-dcache-load-misses ./counter -t 10 -i=1000 --bar=<sense, pthread> -o out.txt`  
`perf stat -e L1-dcache-loads -e L1-dcache-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=<sense, pthread>`
#### For branch-prediction hit rate
`perf stat -e branch-loads -e branch-load-misses ./counter -t 10 -i=1000 --bar=<sense, pthread> -o out.txt`  
`perf stat -e branch-loads -e branch-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=<sense, pthread>`
#### For L1 cache hit rate
`perf stat -e page-faults ./counter -t 10 -i=1000 --bar=<sense, pthread> -o out.txt`  
`perf stat -e page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=<sense, pthread>`
#### Or all at once
`perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./counter -t 10 -i=1000 --bar=<sense, pthread> -o out.txt`  
`perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=<sense, pthread>`

#### Table (Iteration/Array at 1000, threads at 10)
Program    | Barrier | Run Time (s) | L1 cache hit rate | branch-prediction hit rate | page-fault
:--------- | :-----: | :----------- | :---------------- | :------------------------- | :---------
Counter    | sense   | 0.001862     | 99.70%            | 99.86%                     | 154
Counter    | pthread | 0.012391     | 90.04%            | 99.01%                     | 148
BucketSort | sense   | 0.000154     | 99.32%            | 99.46%                     | 187
BucketSort | pthread | 0.000531     | 96.56%            | 97.58%                     | 186


### Locks
#### For L1 cache hit rate
`perf stat -e L1-dcache-loads -e L1-dcache-load-misses ./counter -t 10 -i=1000 --lock=<tas,ttas,ticket,pthread> -o out.txt`  
`perf stat -e L1-dcache-loads -e L1-dcache-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=<tas,ttas,ticket,pthread>`
#### For branch-prediction hit rate
`perf stat -e branch-loads -e branch-load-misses ./counter -t 10 -i=1000 --lock=<tas,ttas,ticket,pthread> -o out.txt`  
`perf stat -e branch-loads -e branch-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=<tas,ttas,ticket,pthread>`
#### For L1 cache hit rate
`perf stat -e page-faults ./counter -t 10 -i=1000 --lock=<tas,ttas,ticket,pthread> -o out.txt`  
`perf stat -e page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=<tas,ttas,ticket,pthread>`
#### Or all at once
`perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./counter -t 10 -i=1000 --lock=<tas,ttas,ticket,pthread> -o out.txt`  
`perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=<tas,ttas,ticket,pthread>`

#### Table (Iteration/Array at 1000, threads at 10)
Program    | Lock    | Run Time (s) | L1 cache hit rate | branch-prediction hit rate | page-fault
:--------- | :------ | :----------- | :---------------- | :------------------------- | :---------
Counter    | tas     | 0.000574     | 92.37%            | 97.01%                     | 144
Counter    | ttas    | 0.000705     | 93.40%            | 97.07%                     | 145
Counter    | ticket  | 0.001021     | 96.17%            | 98.69%                     | 145
Counter    | pthread | 0.001050     | 97.06%            | 99.01%                     | 145
BucketSort | tas     | 0.000346     | 96.99%            | 97.75%                     | 171
BucketSort | ttas    | 0.000371     | 97.02%            | 97.78%                     | 170
BucketSort | ticket  | 0.000358     | 97.30%            | 97.92%                     | 166
BucketSort | pthread | 0.000349     | 96.53%            | 97.46%                     | 171


#### 🐜 Surviving Bugs
  - Input/output files are loaded/saved as is. Does not check for txt file types or if data is integers.
  - Joining of threads is out of order for locks, so sometimes fails bucket sorting.

#### Resources:
1. [bucket sort (variaion)](https://gist.github.com/benapier/3df28b56946ae6a550805acca6f1db34)
2. [multithreading pthread](https://www.bogotobogo.com/cplusplus/multithreading_pthread.php)
3. [pthread locks](https://randu.org/tutorials/threads/)
3. [mutex locks for linux threads](https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/)
1. [perf](https://perf.wiki.kernel.org/index.php/Tutorial) [perf stat](https://man7.org/linux/man-pages/man1/perf-stat.1.html)
1. [measuring execution time](https://solarianprogrammer.com/2019/04/17/c17-programming-measuring-execution-time-delaying-program/)
2. [clock and time functions](https://linux.die.net/man/2/clock_gettime)
3. [chrono high resolution time accuracy](https://www.tutorialspoint.com/how-to-create-a-high-resolution-timer-with-cplusplus-and-linux)
4. [atomic fetch add](https://apimirror.com/c/atomic/atomic_fetch_add) - since fai not available
10. [Advantages of vector over array in C++](https://www.geeksforgeeks.org/advantages-of-vector-over-array-in-c/)
9. Class lecture slides, along with 80+ hours of 'tinkering'
