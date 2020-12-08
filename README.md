# Concurrent Programming

*Potential for multiple operations to run at the same time (parallelism), but doesn't mean it will.*
*Concurrency ≠ Parallelism*

## Table on Contents
1. [Sorting Unique Integers](Sorting_Unique_Integers) - Using mergesort and quicksort (serial)
2. [Merge Sort with Fork/Join and Bucket Sort using locks](FJMerge_And_LBucket_Sort) - Using parallelism, mergesort with fork/join and bucketsort with locks
3. [Locking Algorithms](Locking_Algorithms) - Multithreading on an atomic global counter and bucket sorting using various barrier and locking methods
4. [OpenMP](OpenMP_Mergesort) - Parallel merge sorting using the application programming interface OpenMP (Open Multi-Processing)
5. [Concurrent Containers](Concurrent_Containers) - Various multi-threaded stack (S) and queue (Q) algorithms such as Treiber (S), Michael & Scott (Q), Single Global Lock (S & Q), Baskets (Q), and an attempt at Elimination (S)