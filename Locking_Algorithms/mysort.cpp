/*
 * mysort.cpp
 * sorting integers based on bucket sort, using bar and lock arguments
 * * that choose which barrier and lock to use
 * Upon sorting, all integers will be saved to argument specified file
 */

#define _POSIX_C_SOURCE 200112L
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>
#include <mutex>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include "pthread_add.h" // for when running on macOS
#include "arg_parser.h"

#define SEQ_CST memory_order_seq_cst
#define RELAXED memory_order_relaxed

using namespace std;

pthread_t* threads;
size_t* args;
size_t NUM_THREADS, NUM_ITERATIONS;
pthread_barrier_t bar;
pthread_mutex_t mutexLock;
atomic<int> next_num = 0, now_serving = 0, atomicTID = 0;
atomic<int> sense = 0, cnt = 0, count = 0;
atomic<bool> tas_flag = 0;
int arraysize, arr[100000], thread_num;
int COUNTER = 0;
int numberOver = 0;
atomic<int> b_count (0);
mutex b_lock; 					// lock for bucket sorting

/* execution time struct */
typedef chrono::high_resolution_clock Clock;


/* ===================== MAIN ==================== */
int main(int argc, const char* argv[]){
  struct arg_params args_parsed = arg_parser(argc, argv);
  string inputFile = args_parsed.inputFile;
	string outputFile = args_parsed.outputFile;
	NUM_THREADS = args_parsed.NUM_THREADS;
	string algorithm = args_parsed.algorithm;
	int bar_arg = args_parsed.bar;
  int lock_arg = args_parsed.lock;

  // init
  threads = static_cast<pthread_t*>(malloc(NUM_THREADS*sizeof(pthread_t)));
	args = static_cast<size_t*>(malloc(NUM_THREADS*sizeof(size_t)));
	pthread_barrier_init(&bar, NULL, NUM_THREADS);

	// create array from input file
	fstream file(inputFile.c_str(), ios_base::in);
	int a, b = 0;
	arraysize = 0;
	string line;
	while (getline(file, line)) arraysize++;
	arr[arraysize];
	fstream infile(inputFile, ios_base::in);
	while (infile >> a) { arr[b] = a; b++; }

  // execution start time
  auto start_time = Clock::now();

  /* argument statement (from parser) is as follows:
   * bar: 1-sense, 2-pthread
   * lock: 1-tas, 2-ttas, 3-ticket, 4-pthread */
  switch (bar_arg) {
    // bar sense
    case 1:
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_create(&threads[i], NULL, counter_sense, (void*)NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_join(threads[i], NULL);
      break;
    // bar pthread
    case 2:
      pthread_barrier_init(&bar, NULL, NUM_THREADS);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_create(&threads[i], NULL, counter_bar_pthread, (void*)NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_join(threads[i], NULL);
      pthread_barrier_destroy(&bar);
      break;
		// something didn't match up
    default:
      printf("An error occured in BAR switch.");
      exit(-1);
	}

	switch (lock_arg) {
    // lock tas
    case 1:
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_create(&threads[i], NULL, counter_TAS, (void*)NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_join(threads[i], NULL);
      break;
    // lock ttas
    case 2:
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_create(&threads[i], NULL, counter_TTAS, (void*)NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_join(threads[i], NULL);
      break;
    // lock ticket
    case 3:
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_create(&threads[i], NULL, counter_ticket_lock, (void*)NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_join(threads[i], NULL);
      break;
    // lock pthread
    case 4:
      pthread_mutex_init(&mutexLock, NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_create(&threads[i], NULL, counter_lock_pthread, (void*)NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_join(threads[i], NULL);
      pthread_mutex_destroy(&mutexLock);
      break;
    // something didn't match up
    default:
      printf("An error occured in LOCK switch.");
      exit(-1);
  }

  // execution end time
  auto end_time = Clock::now();
  // unsigned int 4,294,967,295, which is only 4.3 seconds
  // unsigned long, plan on never running out (over 5 centuries)
  unsigned long time_spent = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
  printf("Time elapsed is %lu nanoseconds\n", time_spent);
  printf("                %f seconds\n", time_spent/1e9);


	/* WRITE SORTED ARRAY TO FILE */
	ofstream outfile;
	outfile.open(outputFile);
	for (int i = 0; i < arraysize; i++) outfile << arr[i] << endl;
	outfile.close();

	free(threads);
	free(args);
	pthread_barrier_destroy(&bar);
}

/* *************** BAR FUNCTIONS *************** */
/* ****** sense ****** */
void sense_wait() {
  int n = NUM_THREADS;

  thread_local bool cur_sense = 0;
  cur_sense = !cur_sense;
  int cnt_copy = atomic_fetch_add(&cnt, 1);
  if (cnt_copy == n - 1) {
    cnt.store(0, memory_order_relaxed);
    sense.store(cur_sense);
  } else while(sense.load() != cur_sense);
}

void *counter_sense(void *) {
  int tid = atomicTID++;
  for(int i = 0; i < NUM_ITERATIONS; ++i) {
    // COUNTER++;
    sense_wait();
  }
}

/* ****** pthread ****** */
void *counter_bar_pthread(void *) {
  int tid = atomicTID++;
  pthread_barrier_wait(&bar);
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    // COUNTER++;
    pthread_barrier_wait(&bar);
  }
}

/* *************** LOCK FUNCTIONS *************** */
/* ****** tas ****** */
bool tas() {
  if (tas_flag == false){
    tas_flag = true;
    return true;
  } else return false;
}

void tas_lock() {
  while(tas() == false) {}
}

void tas_unlock() {
  tas_flag.store(false, SEQ_CST);
}

void *counter_TAS(void *) {
  for(int i = 0; i < NUM_ITERATIONS; ++i) {
    tas_lock();
    // COUNTER++;
    tas_unlock();
  }
}

/* ****** ttas ****** */
void ttas_lock() {
  while(tas_flag.load(SEQ_CST) == true
        || tas() == false) {}
}

void *counter_TTAS(void *) {
  ttas_lock();
  for(int i = 0; i < NUM_ITERATIONS; ++i) {
    // COUNTER++;
    tas_unlock();
    ttas_lock();
  }
  tas_unlock();
}

/* ****** ticket ****** */
void ticket_lock() {
  // fai is fetch and increment, but not in C++?
  int num = atomic_fetch_add(&next_num, 1);
  while(now_serving.load(SEQ_CST) != num) {
    if (COUNTER > NUM_ITERATIONS*NUM_THREADS) {
      numberOver = 1;
      break;
    }
  }
}

void ticket_unlock() {
  // fai is fetch and increment, but not in C++?
  atomic_fetch_add(&now_serving, 1);
}

void *counter_ticket_lock(void *) {
  ticket_lock();
  for(int i = 0; i < NUM_ITERATIONS; ++i) {
    if (!numberOver) COUNTER++;
    ticket_unlock();
    ticket_lock();
  }
  ticket_unlock();
}

/* ****** pthread ****** */
void *counter_lock_pthread(void *) {
  int tid = atomicTID++;
  for(int i = 0; i < NUM_ITERATIONS; ++i) {
    while (COUNTER % (NUM_THREADS - 1) != tid) {
      if (COUNTER >= NUM_ITERATIONS*NUM_THREADS) break;
    }
    pthread_mutex_lock(&mutexLock);
    COUNTER++;
    pthread_mutex_unlock(&mutexLock);
  }
}

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ BUCKET SORT ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
// Function to sort arr[] from [left to right] bucket sort
void bucketSort(int low, int high) {
	int i, j, max_value = 0;

	// find max key value in arr
	for (i = low; i <= high; ++i)
		if (arr[i] > max_value) max_value = arr[i];

	// create n empty local buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	// put array elements in different buckets
	for (i = low; i <= high; ++i)
		buckets[arr[i]]++;


	// concatenate all buckets into arr[]
	for (i = 0; i < buckets.capacity(); ++i)
		for (j = 0; j < buckets[i]; ++j) {
				arr[b_count] = i;
				b_count++;
		}

	//lock_guard<mutex> unlock(b_lock);
}

void* lk_bucketSort(void* args) {
	size_t tid = *((size_t*)args);

	int arrsplit = arraysize / NUM_THREADS;
	int low = tid * arrsplit;
	int high = ((tid + 1) * arrsplit);

	lock_guard<mutex> guard(b_lock);
	bucketSort(low, high);

	return 0;
}
/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END BUCKET SORT ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */