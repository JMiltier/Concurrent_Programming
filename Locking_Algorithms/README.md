# 🔒 C++ barrier & locking algorithms
---
A benchmark (counter) is used to increment a global atomic counter (integer) using various barrier and locking algorithms (listed below). Additionally, the same algorithms are used while using a bucket sort algorithm. This sorting algorithm takes an unsorted (specified) *.txt* file of unique (random) integers and outputs a sorted list of integers to a different (specified) *.txt* file.

#### Counter program (benchmark)
This multithreaded program increments an integer counter based on the number of threads input and the number of iterations for each thread to perform. Ideally, the output should be the number of threads mutliplied by the number of iterations (for example 10 thread * 100 iterations = 1000). The global atomic counter is incremented by all threads in this program. Various barrier and locking algorithms are used, as outlined below. Additionally, this program is meant more for benchmarking performance results using a Linux basd performance analyzing tool called 'perf'. Results are provided further in this document.

#### Bucket sorting program (mysort program)
Using the same algorithms as the benchmark program, this bucket sorting algorithm program reads in a file of randomly sorted integers and stores them into a vector locally. It then sorts based on various barrier and locking algorithms (same as above) while using multiple threads, both of these are specified by a input arguments. Based on the number of threads, the input vector is split up into different buckets and sorted synchronously before rejoining/inserting back into the primary vector. Though the time and memory cost overhead may be significant for high thread numbers and a small list of numbers, it is easily scalable when sorting larger number sets. 

#### Barrier Algorithms:
  1. **Sense-reversal barrier** - Each sequential barrier uses an opposite value of the previous barrier's value as a pass/stop state. By not using the same value, this barrier algorithm solves for potential deadlocking issues. For example, if barrier 1 uses the value 0 to stop the threads, barrier 2 will use 1 to stop, barrier 3 will use 0, barrier 4 will use 1, and so forth. In the algorithm, it simply flips (0/1) the value each time. 
  2. **Pthread barrier** - Implementation of pthread locks is setup from initializing how many threads to expect at the barrier. Once all threads have arrived, the barrier call will *unlock* and the threads will continue until either finished, or another barrier is set. Once the threads are complete, they will join to the main thread as a sense of notification that the worker threads are complete (and then dead).

#### Locking Algorithms:
  1. **Test-and-set (TAS) lock** - using a single atomic value, TAS functionality tests a lock (true/false; obtain when false), and then sets lock. In the algorithm, a constant spinning loop tests against the lock to see when it's set to false (0). And once 0, it obtains the lock by setting the atomic value to 1. One issue with TAS is it's unfairness with not guaranteeing FIFO ordering for threading competing against the lock. This algorithm is easy to implement, but horrible for scaling. 
  2. **Test-and-test-and-set (TTAS) lock** - more elaborate locking based on a modified version of TAS. In additional to test the lock, it also continuously checks the memory to see if the lock has been obtained. Once free, it attempts to take the lock by using test and set. So the additional 'test' comes from testing the memory of the lock, before trying to test and set it. 
  3. **Ticket lock** - a queue like algorithm that uses two atomic counters, one which designates who is up, and who is next. Though this could lead to starvation of some threads, it allows for the next thread (among all) to know it's got CPU access next. This is also a spinlock, where the next in the queue waits until it's equal to the value now available. Both of these counters will increment as the locks/unlocks occur. 
  4. **Pthread lock** - using a single mutex lock and an atomic shared variable, this lock first initializes the mutex lock, and then locks/unlocks (sets) the lock using the built in pthread library. This lock also destroys the lock once all threads have merged back to the master thread.
<div style="page-break-after: always; visibility: hidden">\pagebreak</div>

### Sorting List Structure
  - Migrated from an *array*, and switched to using a *vector*. Some differences (and ultimate reasoning for decision) are outlined below.

  |            | Array   | Vector  | Notes
  |:----        | :---:   | :----:  |  :--
  |Synchronized | No      | Yes     |   1
  |Implemented | Statically or Dynamically | Dynamic arrays with list interface |
  |Size         | Fixed | Dynamic |   2 
  |Deallocation | Explicitly  | Automatically |
  |Size Determination | undetermined | O(1) time | 3
  |Reallocation | Explicitly  | Implicitly  |
  |Returned through func? | ~No | Yes | 4
  |Copied Directly? | No  | Yes | 5
  |Assigned Directly? | No | Yes | 5

  1. Only one thread at a time can access the code with vectors, while arrays allow for multiple threads to work on it at the same time. This is turn causes thread waiting for vectors.
  2. Size: The size of an array is fixed, and expensive to reallocate. Vectors, on the other hand, are resizable since they are allocated on heap memory. 
  3. Cannot be determined if array is dynamically allocated. Vectors are O(1) because they maintain variables which keeps track of container size at all times.
  4. Arrays can only be returned if they are dynamically allocated from a function
  5. Vectors can be copied directly (example: vec1 = vec2), where arrays need to be iterated when copied. Same applied for assignment. (arr1 = arr2 would produce an error)

## Code Organization
---

#### 📁 Files
  1. `Makefile` - used to create and remove executable C++11 objects 
  2. `mysort.cpp` - bucket sort using bars and locks
  3. `arg_parser.h` - parsing and error handling for mysort program input options
  4. `counter.cpp` - benchmark program to incorporate bars and locks
  5. `cnt_arg_parser.h` - parsing and error handling for counter program input options
  6. `README.pdf` - write-up for project (this file)

#### 💾 Additional files
  1. `randomNumberGen.cpp`- creates a text file with a specified number of random numbers from [ 1 - number size ]. Default number is 10,000 unless specified when ran (as second argument).
  2. `pthread_add.h` - threading if compiling on macOS (will still run as normal for linux systems)'
  3. `perf.sh` - shell script file to run perf tests -- Omitted this file b/c was not able to run due to permissions on jupiter notebook. Methods for testing are listed below.

### Compiling
  Note: If zipped, first unzip file before proceeding.
  1. From root directory, run `make` from the terminal. This will generate a program called *counter* and *mysort*
  2. Next, follow execution syntax below for either *counter* or *mysort*.
  3. `Make clean` will remove all files generated by `make`.
  4. Additionally, `make numGen` will generate the executable object to generate random numbers to a .txt file (set to *source.txt*).

## Execution
---
#### Program option/input parameters for `counter`
```shell
> counter [--name]
```
`counter --name`: prints name to console.  
**OR**  
```shell
> counter [-t NUM THREADS] [-i=NUM_ITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>] [-o out.txt]
```
  1. `-t NUM_THREADS`: specify how many threads to use during execution (including master thread)
  2. `-i=NUM_ITERATIONS`: number of times to increment counter on each thread
  3. `--bar=<sense,pthread>`: type of barrier algorithm to use
  4. `--lock=<tas,ttas,ticket,mcs,pthread>`: type of locking algorithm to use
  5. `-o out.txt`: sorted txt file of numebrs, in which the program outputs/writes to
  - **Additional outputs**: time of execution in nanoseconds  
Example of program execution, using 10 threads, 1000 iterations for each thread, TAS locking algorithm, and writing to file out.txt:
```shell
> ./counter -t 10 -i=1000 --lock=tas -o out.txt
```

#### Program option/input argument parameters for `mysort`
```shell
> mysort [--name]
```
`mysort --name`: prints name to console.  
**OR**  
```shell
> mysort [source.txt] [-o out.txt] [-t NUM THREADS] [--alg=<bucket>] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>]
```
  1. `source.txt`: input file for unsorted and random list of numbers, each on a new line
  2. `-o out.txt`: output file for sorted list of numbers (from input file in step preceeding this one), each on a new line
  3. `-t NUM_THREADS`: specify how many threads to use during execution (including master thread)
  4. `--alg=<bucket>`: sorting algorithm to use (always bucket)
  5. `--bar=<sense,pthread>`: type of barrier algorithm to use
  6. `--lock=<tas,ttas,ticket,mcs,pthread>`: type of locking algorithm to use
  - **Additional outputs**: time of execution in nanoseconds, and if sorting fails  
Example of program execution for mysort, reading from an input file of source.txt, writing to an output file of out.txt, using bucket sort, a sense-reversal barrier, and 5 threads:
```shell
> ./mysort source.txt -o out.txt -t 5 --alg=bucket --bar=sense
```

## Analyzing program performances, using `perf`
---
## Barriers
#### For L1 cache hit rate
```shell
> perf stat -e L1-dcache-loads -e L1-dcache-load-misses ./counter -t 10 -i=1000 --bar=<sense, pthread> -o out.txt
> perf stat -e L1-dcache-loads -e L1-dcache-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=<sense, pthread>
```
#### For branch-prediction hit rate
```shell
> perf stat -e branch-loads -e branch-load-misses ./counter -t 10 -i=1000 --bar=<sense, pthread> -o out.txt
> perf stat -e branch-loads -e branch-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=<sense, pthread>
```
#### For L1 cache hit rate
```shell
> perf stat -e page-faults ./counter -t 10 -i=1000 --bar=<sense, pthread> -o out.txt
> perf stat -e page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=<sense, pthread>
```
#### Or all stats at once (how I ended up doing it), using 10 repeated runs
```shell
> perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./counter -t 10 -i=1000 --bar=<sense, pthread> -o out.txt  
> perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --bar=<sense, pthread>
```

### Barrier Perf Table (Iteration/Array at 1000, 10 threads, and 10 repeated runs (average))
Program | Barrier | Run Time (s) | L1 cache hit rate | branch-prediction hit rate | page-fault
:------ | :-----: | :----------- | :---------------- | :------------------------- | :---------
counter | sense   | 0.001862     | 99.70%            | 99.86%                     | 154
counter | pthread | 0.012391     | 90.04%            | 99.01%                     | 148
mysort  | sense   | 0.001578     | 99.32%            | 99.46%                     | 187
mysort  | pthread | 0.000531     | 96.56%            | 97.58%                     | 186

## Locks
#### For L1 cache hit rate
```shell
> perf stat -e L1-dcache-loads -e L1-dcache-load-misses ./counter -t 10 -i=1000 --lock=<tas,ttas,ticket,pthread> -o out.txt
> perf stat -e L1-dcache-loads -e L1-dcache-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=<tas,ttas,ticket,pthread>
```
#### For branch-prediction hit rate
```shell
> perf stat -e branch-loads -e branch-load-misses ./counter -t 10 -i=1000 --lock=<tas,ttas,ticket,pthread> -o out.txt
> perf stat -e branch-loads -e branch-load-misses ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=<tas,ttas,ticket,pthread>
```
#### For L1 cache hit rate
```shell
> perf stat -e page-faults ./counter -t 10 -i=1000 --lock=<tas,ttas,ticket,pthread> -o out.txt
> perf stat -e page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=<tas,ttas,ticket,pthread>
```
#### Or all stats at once (how I ended up doing it), using 10 repeated runs
```shell
> perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./counter -t 10 -i=1000 --lock=<tas,ttas,ticket,pthread> -o out.txt 
> perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./mysort source.txt -o out.txt -t 10 --alg=bucket --lock=<tas,ttas,ticket,pthread>
```

### Lock Perf Table (Iteration/Array at 1000, 10 threads, and 10 repeated runs (average))
Program | Lock    | Run Time (s) | L1 cache hit rate | branch-prediction hit rate | page-fault
:--------- | :------ | :----------- | :---------------- | :------------------------- | :---------
counter | tas     | 0.000574     | 92.37%            | 97.01%                     | 144
counter | ttas    | 0.000705     | 93.40%            | 97.07%                     | 145
counter | ticket  | 0.001021     | 96.17%            | 98.69%                     | 145
counter | pthread | 0.001050     | 97.06%            | 99.01%                     | 145
mysort  | tas     | 0.000346     | 96.99%            | 97.75%                     | 171
mysort  | ttas    | 0.000371     | 97.02%            | 97.78%                     | 170
mysort  | ticket  | 0.000358     | 97.30%            | 97.92%                     | 166
mysort  | pthread | 0.000349     | 96.53%            | 97.46%                     | 171

### Perf Analysis
Overall among benchmarking and mysort performance tests, locking algorithms seem to be faster and have better hit rates. Speculating that with barriers, each process has to wait for one another before proceeding on, while the locking algorithms are able to sort the subarray independently on one another prior to merging back. The pthread appeared the perform the worse across the board, since each thread was having to synchronize after every bit, there's a lot of overhead cost involed. The fastest looked to be TAS lock, imagining it's the simplest of the locks, but it not scalable so could see degraded results with larger numbers. 

#### 🐜 Surviving Bugs
  - Input/output files are loaded/saved as is. Does not check for txt file types or if data is integers.
  - Creating/Joining of threads is out of order for locks, so sometimes fails bucket sorting.

#### Resources:
1. [bucket sort (variaion)](https://gist.github.com/benapier/3df28b56946ae6a550805acca6f1db34)
2. [multithreading pthread](https://www.bogotobogo.com/cplusplus/multithreading_pthread.php)
3. [pthread locks](https://randu.org/tutorials/threads/)
4. [mutex locks for linux threads](https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/)
5. [pthreads](https://computing.llnl.gov/tutorials/pthreads/)
6. [test and set spinlocks](https://geidav.wordpress.com/2016/03/23/test-and-set-spinlocks/) 
7. [TAS spinlock 2](https://rigtorp.se/spinlock/)
8. [Test and test-and-set (wiki)](https://en.wikipedia.org/wiki/Test_and_test-and-set)
9. [Double-checked locking](http://www.cs.umd.edu/~pugh/java/memoryModel/DoubleCheckedLocking.html)
10. [Ticket lock](https://alexmeuer.com/blog/ticket-algorithm)
11. [Ticket lock (wiki)](https://en.wikipedia.org/wiki/Ticket_lock)
12. [Sense Reversal (small sample)](https://www2.cs.duke.edu/courses/cps221/spring99/lectures/week8-l1/tsld019.htm)
13. [Pthread barrier](https://man7.org/tlpi/code/online/dist/threads/pthread_barrier_demo.c.html)
14. [Pthread barrier 2 (in C)](https://man7.org/tlpi/code/online/dist/threads/pthread_barrier_demo.c.html)
15. [perf](https://perf.wiki.kernel.org/index.php/Tutorial) & [perf stat](https://man7.org/linux/man-pages/man1/perf-stat.1.html)
16. [measuring execution time](https://solarianprogrammer.com/2019/04/17/c17-programming-measuring-execution-time-delaying-program/)
17. [clock and time functions](https://linux.die.net/man/2/clock_gettime)
18. [chrono high resolution time accuracy](https://www.tutorialspoint.com/how-to-create-a-high-resolution-timer-with-cplusplus-and-linux)
19. [atomic fetch add](https://apimirror.com/c/atomic/atomic_fetch_add) - since 'fai' not available
20. [Advantages of vector over array in C++](https://www.geeksforgeeks.org/advantages-of-vector-over-array-in-c/)
21. [Awesome videos on locks and atomics](https://herbsutter.com/2013/02/11/atomic-weapons-the-c-memory-model-and-modern-hardware/)
22. Class lecture slides, along with 80+ hours of 'tinkering'

<!-- CREATE README PDF -->
<!-- md2pdf README.md --highlight-style atom-one-dark --pdf-options '{ "format": "Letter", "margin": "20mm", "printBackground": true }' -->