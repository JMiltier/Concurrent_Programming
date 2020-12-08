# C++ Parallel Algorithms

#### Concurrent Tree
A tree, whether a binary search tree (BST), radix tree, red-black tree (RBT), or any other tree imagineable, allowing for multiple reads/writes/deletes functionalities for parallism. Ideally, all reads are lock-free, meaning any read threads should not block. These read threads should always see a consistent version of the tree, and should never block writing threads. As for the writing threads, they should block other writing threads, but not any reading threads. Writing threads blocking writing threads ensures consistency when modifying the tree. [Additional info](https://www.cs.cmu.edu/~yihans/papers/tutorial.pdf) and for [BST](https://stanford-ppl.github.io/website/papers/ppopp207-bronson.pdf)

#### Concurrent Containers
Several concurrent algorithms with an elimination: SGL stack and queue, Treiber stack, M&S stack, baskets queue, and elimination SGL/Treiber stacks. 

#### Transactions
A series of code execution that only commits all if the entire run is completed successfully. Othwerise, it will abort (fail) and roll back. Doing this in parallel seems odd, being that it may be need to wait on one (sequentially) function to run before another. This seems extremely useful for database management. More importantly, there seems that there may be some additional troubleshooting for transactions that may not be discovered until further implemented. [Additional read](https://ashutoshtripathi.com/2017/11/28/concurrent-executions-in-transaction/)

#### Decision of Parallel Algorithm to implement
For the sake of resources available, and class slides, concurrent containers seemed to have the greatest amount of references. As discovered, these are added to the resources section at the end of this document. 

## Algorithm Descriptions
#### Single Global Lock (SGL) Stack (LIFO)
Lock-based stack that uses a single global lock, which can decrease performance time when scaling.
#### Single Global Lock (SGL) Queue (FIFO)
Lock-based queue that also uses a single global local, again at a degradation issue with increasing complexity. 
#### Treiber Stack (LIFO)
The Treiber stack algorithm is a scalable lock-free stack utilizing the fine-grained concurrency primitive compare-and-swap. It is believed that R. Kent Treiber  The Treiber stack algorithm is a scalable lock-free stack utilizing the fine-grained concurrency primitive compare-and-swap. It is believed that R. Kent Treiber was the first to publish it in his 1986 article "Systems Programming: Coping with Parallelism". However, treiber stack is prone to bottlenecking and thus offers little scalability. 
#### Michael and Scott (MS) Queue (FIFO)
Non-blocking, dynamic-memory algorithm that whose threads reserve blocks beforehand, so that other threads won't reclaim before it finishes. This comes increased performance overhead. 
#### Baskets Queue (FIFO)
A concurrent lock-free dynamic linearizable queue algorithm. The throughput of this is greater than the previous ones listed - as it created a mixed order (baskets) of items, instead of the traditional ordered list. Operations within each basket can be performed in parallel.
#### Elimination Stacks (FIFO) with SGL Stack and Treiber Stack -- !Not fully implemented
Concurrent stack algorithm that is lock-free (robust), parallel (scalable), and adaptive (performs well at low loads). Additionally, it is linearizable and combine modularly with other algorithms. Notes from paper: A push followed by a pop does not change a data structure's state. If the two threads can exchange these values without having to touch a centralized structure, they have 'eliminated' the push->pop effect. The idea: use a single elimination array as a backoff scheme on a shared lock-free stack. If the thread fails on the stack, it attempts to eliminate on the array, and if they fail at elimination, they attempt to access the stack again (and so on).  
Global arrays:
- location[1..n] has an element per thread, and pointer location to thread
- collision[1..size] holds ids of the threads trying to collide
Vars:
- p, q - thread id
Order of eliminate stack:
1. tries to perform operation on central stack
2. if that fails, it goes through the collision layer
    1. writes thread info to location array
    2. chooses random location in collision array
    3. read random location and store it, while trying to write own id
    4. will keep trying 2c until successful

#### Flat combining Stack and Queue 
Wasn't able to find much info on implementing this.
FC resources: [FC stack](http://libcds.sourceforge.net/doc/cds-api/classcds_1_1intrusive_1_1_f_c_stack.html), [FC Queue](http://libcds.sourceforge.net/doc/cds-api/classcds_1_1intrusive_1_1_f_c_queue.html) [FC and trade-off](https://sites.google.com/site/cconcurrencypackage/flat-combining-and-the-synchronization-parallelism-tradeoff) [FC chat](https://kukuruku.co/post/lock-free-data-structures-the-evolution-of-a-stack/#flat-combining)

## Code Organization
#### 📁 Files
  1. `Makefile` - used to create and remove executable C++11 objects 
  2. `container.cpp` - main file
  3. `container.h` - header file for main, includes testing
  4. `arg_parser.h` - parsing and error handling for mysort program input options
  5. `sgl_stack.h` - functionality for stack using a single global lock
  6. `sgl_queue.h` - functionality for queue using a single global lock
  7. `treiber_stack.h` - functionality for treiber stack
  8. `ms_queue.h` - functionality for Michael and Scott stack
  9. `baskets_queue.h` - functionality for Baskets queue
  10. `elim_stack.h` - added functionality for stacks above, using threads to exchange values through elimination
  11. `README.pdf` - write-up for project (this file)

#### 💾 Additional files
  1. `randomNumberGen.cpp`- creates a text file with a specified number of random numbers from [ 1 - number size ]. Default number is 10,000 unless specified when ran (as second argument).
  2. `pthread_add.h` - threading if compiling on macOS (will still run as normal for linux systems)'

### Compiling
  Note: If zipped, first unzip file before proceeding.
  1. From root directory, run `make` from the terminal. This will generate a program called *counter* and *mysort*
  2. Next, follow execution syntax below for either *counter* or *mysort*.
  3. `Make clean` will remove all files generated by `make`.
  4. Additionally, `make numGen` will generate the executable object to generate random numbers to a .txt file (set to *source.txt*).

<div style="page-break-after: always; visibility: hidden">\pagebreak</div>

## Execution
#### Program option/input parameters for `containers`
```shell
> containers [--name]
```
`containers --name`: prints name to console.
```shell
> containers [-h]
```
`containers -h`: prints help instructions to console.
```shell
> containers [--test]
```
`containers --test`: visual tests using a vector with 15 integers against 1, 3, and 5 threads for each algorithm. This currently only works on macOS (tested on v10.15.7)
**OR**  
```shell
> containers <input_file> [-t NUM THREADS] --algorithm=<sgl_stack,sgl_queue,treiber_stack,ms_queue,baskets_queue, elim_sql_stack, elim_t_stack>
```
  1. `input_file`: name of file with ints 
  2. `-t NUM_THREADS`: specify how many threads to use during execution (including master thread)
  3. `--container=<sgl_stack,sgl_queue,treiber_stack,ms_queue,baskets_queue, elim_sql_stack, elim_t_stack>`: type of algorithm to use
  - **Additional outputs**: time of execution in nanoseconds  
Example of program execution, using integers from source.txt file, 5 threads, and the sgl_stack algorithm
```shell
> ./containers source.txt -t 5 --algorithm=sgl_stack
```
<div style="page-break-after: always; visibility: hidden">\pagebreak</div>

## Analyzing program performances, using `perf`
#### For L1 cache hit rate
```shell
> perf stat -e L1-dcache-loads -e L1-dcache-load-misses ./containers source.txt -t 5 --algorithm=<sgl_stack,sgl_queue,treiber_stack,ms_queue,baskets_queue, elim_sql_stack, elim_t_stack>
```
#### For branch-prediction hit rate
```shell
> perf stat -e branch-loads -e branch-load-misses ./containers source.txt -t 5 --algorithm=<sgl_stack,sgl_queue,treiber_stack,ms_queue,baskets_queue, elim_sql_stack, elim_t_stack>
```
#### For page faults
```shell
> perf stat -e page-faults ./containers source.txt -t 5 --algorithm=<sgl_stack,sgl_queue,treiber_stack,ms_queue,baskets_queue, elim_sql_stack, elim_t_stack>
```
#### Or all stats at once (how I ended up doing it), using 10 repeated runs
```shell
> perf stat --repeat 10 -e L1-dcache-loads,L1-dcache-load-misses,branch-loads,branch-load-misses,page-faults ./containers source.txt -t 5 --algorithm=<sgl_stack,sgl_queue,treiber_stack,ms_queue,baskets_queue, elim_sql_stack, elim_t_stack>
```

<style> table { font-size: 12px } </style>
### Container Perf Table (Iteration/Array of 10000, 5 threads, and 10 repeated runs (average))
Algorithm           | Run Time (s) | L1 cache hit rate | branch-prediction hit rate | page-fault
:------------------ | :----------- | :---------------- | :------------------------- | :---------
SGL Stack           | 0.003164545  | 98.46%            | 99.05%                     | 134
SGL Queue           | 0.003159215  | 98.44%            | 99.05%                     | 133
Treiber Stack       | 0.003125873  | 98.44%            | 99.05%                     | 134
MS Queue            | 0.002613348  | 98.48%            | 99.05%                     | 134
Baskets Queue       | 0.002597239  | 98.44%            | 99.06%                     | 134
Elim SGL Stack      | 0.002524128  | 98.44%            | 99.06%                     | 134
Elim Treiber Stack  | 0.002423572  | 98.45%            | 99.04%                     | 134

##### *NOTE: Run time includes testing

### Perf Analysis
Overall among all of the algorithm's performance tests, the baskets queue algorithms seem to be faster. However, all have very similar hit rates. Ideally, this run times make sense, as the single global locks should perform much slower than the other algorithms since these are sequential operations; each process has to wait for one another before proceeding on. The stacks with elimination (as read, but not implemented) should outperform the SGL and Treiber stacks. 

<div style="page-break-after: always; visibility: hidden">\pagebreak</div>

## Unit Testing
Local test cases were setup on a MacOS machine, and currently unable to run on Linux (seg faults). Below is the console output from a Mac notebook:
```shell
================ STARTING TEST CASES ================

---------------- Testing with 1 Thread ----------------
With Nums: { 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 }
•••••• SGL Stack Tests ••••••
 Pushed: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
 Popped: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 
 Time elapsed: 0.000117 seconds
•••••• SGL Queue Tests ••••••
 Enqueued: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
 Dequeued: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
 Time elapsed: 0.000060 seconds
•••••• Treiber Stack Tests ••••••
 Pushed: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
 Popped: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 
 Time elapsed: 0.000061 seconds
•••••• Michael And Scott Queue Tests ••••••
 Enqueued: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
 Dequeued: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
 Time elapsed: 0.000043 seconds
•••••• Baskets Queue Tests ••••••
 Enqueued: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
 Dequeued: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
 Time elapsed: 0.039010 seconds
 •••••• Elimination SGL Stack Tests ••••••
 Pushed: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
 Popped: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 
 Time elapsed: 0.000054 seconds
•••••• Elimination Treiber Stack Tests ••••••
 Pushed: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
 Popped: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 
 Time elapsed: 0.000066 seconds

---------------- Testing with 3 Threads ----------------
With Nums: { 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 }
•••••• SGL Stack Tests ••••••
 Pushed: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 
 Popped: 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 
 Time elapsed: 0.000357 seconds
•••••• SGL Queue Tests ••••••
 Enqueued: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 
 Dequeued: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 
 Time elapsed: 0.000340 seconds
•••••• Treiber Stack Tests ••••••
 Pushed: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 
 Popped: 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 
 Time elapsed: 0.000117 seconds
•••••• Michael And Scott Queue Tests ••••••
 Enqueued: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 
 Dequeued: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 
 Time elapsed: 0.000209 seconds
•••••• Baskets Queue Tests ••••••
 Enqueued: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 
 Dequeued: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 
 Time elapsed: 0.000103 seconds
 •••••• Elimination SGL Stack Tests ••••••
 Pushed: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 
 Popped: 30 29 28 27 25 24 23 22 26 21 20 19 18 17 16 
 Time elapsed: 0.000150 seconds
•••••• Elimination Treiber Stack Tests ••••••
 Pushed: 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 
 Popped: 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 
 Time elapsed: 0.000095 seconds

---------------- Testing with 5 Threads ----------------
With Nums: { 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 }
•••••• SGL Stack Tests ••••••
 Pushed: 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 
 Popped: 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 
 Time elapsed: 0.000355 seconds
•••••• SGL Queue Tests ••••••
 Enqueued: 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 
 Dequeued: 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 
 Time elapsed: 0.000144 seconds
•••••• Treiber Stack Tests ••••••
 Pushed: 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 
 Popped: 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 
 Time elapsed: 0.000138 seconds
•••••• Michael And Scott Queue Tests ••••••
 Enqueued: 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 
 Dequeued: 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 
 Time elapsed: 0.000125 seconds
•••••• Baskets Queue Tests ••••••
 Enqueued: 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 
 Dequeued: 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 
 Time elapsed: 0.000120 seconds
 •••••• Elimination SGL Stack Tests ••••••
 Pushed: 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 
 Popped: 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 
 Time elapsed: 0.000131 seconds
•••••• Elimination Treiber Stack Tests ••••••
 Pushed: 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 
 Popped: 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 
 Time elapsed: 0.000123 seconds

================ END TEST CASES ================
```

#### 🐜 Surviving Bugs
  - Input/output files are loaded/saved as is. Does not check for txt file types or if data is integers.
  - Creating/Joining of threads is out of order, so sometimes fails pushing/pushing OR enqueuing dequeuing order. 

#### Resources:
1. [*Scalable Lock-free Stack Algorithm](https://people.csail.mit.edu/shanir/publications/Lock_Free.pdf)
2. [Flat Combining the Synchronization-Parallelism Tradeoff](http://mcg.cs.tau.ac.il/papers/spaa2010-fc.pdf)
3. [The Baskets Queue](https://people.csail.mit.edu/shanir/publications/Baskets%20Queue.pdf) 
4. [Concurrent Queue](https://github.com/cameron314/concurrentqueue)
5. [A queue among three threads](https://www.geeksforgeeks.org/sharing-queue-among-three-threads/)
6. [Lock-free programming](http://www.cs.cmu.edu/afs/cs/academic/class/15418-f18/www/lectures/17_lockfree.pdf)
7. [Multi-threading](https://www.bogotobogo.com/cplusplus/multithreaded4_cplusplus11B.php)
8. [Stack push](https://www.cplusplus.com/reference/stack/stack/push/) & [pop](https://www.cplusplus.com/reference/stack/stack/pop/) 
9. [Queue push](https://www.cplusplus.com/reference/queue/queue/push/) & [pop](https://www.cplusplus.com/reference/queue/queue/pop/)
10. [Wiki Treiber Stack](https://en.wikipedia.org/wiki/Treiber_stack)
11. [Treiber Stack]()
12. [Lock-Free Programming](https://www.cs.cmu.edu/~410-s05/lectures/L31_LockFree.pdf)
13. [A Scalable Correct Time-Stamped Stack](https://dl.acm.org/doi/pdf/10.1145/2775051.2676963)
14. [Old counted ptr header file (not currently implemented)](http://ootips.org/yonat/4dev/counted_ptr.h)
15. [cpp compare exchange](https://en.cppreference.com/w/cpp/atomic/atomic_compare_exchange)
16. [stack overflow CAS solution for baskets queue](https://stackoverflow.com/questions/27919854/compare-and-swap-in-c)
17. [nanoseconds sleep for backoff scheme](https://stackoverflow.com/questions/7684359/how-to-use-nanosleep-in-c-what-are-tim-tv-sec-and-tim-tv-nsec)

<!-- CREATE README PDF -->
<!-- md2pdf README.md --highlight-style atom-one-dark --pdf-options '{ "format": "Letter", "margin": "20mm", "printBackground": true }' -->