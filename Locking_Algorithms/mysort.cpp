/*
 * mysort.cpp
 * sorting integers based on bucket sort, using bar and lock arguments
 * * that choose which barrier and lock to use
 * Upon sorting, all integers will be saved to argument specified file
 * Final check of sorted array will compare against sort() algo
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
int NUM_THREADS = 0;
pthread_barrier_t bar;
pthread_mutex_t mutexLock;
atomic<int> sense (0), cnt (0), count (0);
atomic<int> next_num (0), now_serving (0), atomicTID (0);
atomic<bool> tas_flag (0);
int arr[100000], arraysize = 0;
atomic<int> b_count (0);
mutex b_lock;					 // lock for bucket sorting

/* execution time struct */
typedef chrono::high_resolution_clock Clock;

/* bar functions */
void sense_wait();
void *bucketSort_sense(void *args);
void *bucketSort_bar_pthread(void *args);
/* lock functions */
bool tas();
void tas_lock();
void tas_unlock();
void *bucketSort_TAS(void *args);
void ttas_lock();
void *bucketSort_TTAS(void *args);
void ticket_lock();
void ticket_unlock();
void *bucketSort_ticket_lock(void *args);
void *bucketSort_lock_pthread(void *args);

/* bucket sort functions */
void bucketSort_sense_fn(int low, int high, int tid, void *args);
void bucketSort_bar_pthread_fn(int low, int high, int tid, void *args);
void bucketSort_TAS_fn(int low, int high, int tid, void *args);
void bucketSort_TTAS_fn(int low, int high, int tid, void *args);
void bucketSort_ticket_lock_fn(int low, int high, int tid, void *args);
void bucketSort_lock_pthread_fn(int low, int high, int tid, void *args);
void bucketSort(int low, int high);
void* lk_bucketSort(void* args);
/* array checker */
void arrayCheck(int asize, int *a1, int *a2);


/* ===================== MAIN ==================== */
int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	string inputFile = args_parsed.inputFile;
	string outputFile = args_parsed.outputFile;
	NUM_THREADS = args_parsed.NUM_THREADS;
	// string algorithm = args_parsed.algorithm;
	int argument = args_parsed.argument;

	// init memory allocations
	threads = static_cast<pthread_t*>(malloc(NUM_THREADS*sizeof(pthread_t)));
	args = static_cast<size_t*>(malloc(NUM_THREADS*sizeof(size_t)));

	// read in array of integers from input file
	fstream file(inputFile.c_str(), ios_base::in);
	int a, b = 0;
	string line;
	while (getline(file, line)) arraysize++;
	// arr[arraysize];
	int arrCheck[arraysize];
	fstream infile(inputFile, ios_base::in);
	while (infile >> a) {
		arr[b] = arrCheck[b] = a;
		b++;
	}

	// execution start time
	auto start_time = Clock::now();

	/* argument statement (from parser) is as follows:
	 * bar: 1-sense, 2-pthread
	 * lock: 3-tas, 4-ttas, 5-ticket, 6-pthread */
	switch (argument) {
		// bar sense
		case 1:
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_create(&threads[i], NULL, bucketSort_sense, (void*)NULL);
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_join(threads[i], NULL);
			break;
		// bar pthread
		case 2:
			pthread_barrier_init(&bar, NULL, NUM_THREADS);
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_create(&threads[i], NULL, bucketSort_bar_pthread, (void*)NULL);
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_join(threads[i], NULL);
			pthread_barrier_destroy(&bar);
			break;
		// lock tas
		case 3:
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_create(&threads[i], NULL, bucketSort_TAS, (void*)NULL);
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_join(threads[i], NULL);
			break;
		// lock ttas
		case 4:
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_create(&threads[i], NULL, bucketSort_TTAS, (void*)NULL);
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_join(threads[i], NULL);
			break;
		// lock ticket
		case 5:
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_create(&threads[i], NULL, bucketSort_ticket_lock, (void*)NULL);
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_join(threads[i], NULL);
			break;
		// lock pthread
		case 6:
			pthread_mutex_init(&mutexLock, NULL);
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_create(&threads[i], NULL, bucketSort_lock_pthread, (void*)NULL);
			for (int i = 0; i < NUM_THREADS; i ++)
				pthread_join(threads[i], NULL);
			pthread_mutex_destroy(&mutexLock);
			break;
		// something didn't match up
		default:
			printf("An error occured in main argument switch.");
			exit(-1);
	}

	// execute a final sort
	sort(arr, arr+arraysize);
	// execution end time
	auto end_time = Clock::now();
	// unsigned int 4,294,967,295, which is only 4.3 seconds
	// unsigned long, plan on never running out (over 5 centuries)
	unsigned long time_spent = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
	printf("Time elapsed is %lu nanoseconds\n", time_spent);
	printf("               %f seconds\n", time_spent/1e9);

	/* check to make sure the array is sorted as expected */
	sort(arrCheck, arrCheck + arraysize); // sort
	arrayCheck(arraysize, arr, arrCheck); // check against bucketsort

	/* WRITE SORTED ARRAY TO FILE */
	ofstream outfile;
	outfile.open(outputFile);
	for (int i = 0; i < arraysize; i++) outfile << arr[i] << endl;
	outfile.close();

	// free dynamic memory
	free(threads);
	free(args);
}

/* ************************************************************** */
/* ************************* FUNCTIONS ************************** */
/* ************************************************************** */

/* *************** BAR FUNCTIONS *************** */
/* ****** sense ****** */
void sense_wait() {
	thread_local bool cur_sense = 0;
	cur_sense = !cur_sense;
	int cnt_copy = atomic_fetch_add(&cnt, 1);
	if (cnt_copy == NUM_THREADS - 1) {
		cnt.store(0, RELAXED);
		sense.store(cur_sense);
	} else while(sense.load() != cur_sense);
}
void *bucketSort_sense(void *args) {
	int tid = atomicTID++;
	int low = ((tid - 1) * arraysize) / NUM_THREADS;
	int high = ((tid * arraysize) / NUM_THREADS) - 1;
	bucketSort_sense_fn(low, high, tid, args);
	return 0;
}
void bucketSort_sense_fn(int low, int high, int tid, void *args) {
	int max_value = 0;

	// find max key value in arr
	for (int i = low; i <= high; i++)
		if (arr[i] > max_value) max_value = arr[i];

	// create n empty local buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	// make sure all threads are here
	sense_wait();
	// put array elements in different buckets
	for (int i = low; i <= high; i++)
		buckets[arr[i]]++;

	// and then make sure threads are here
	sense_wait();
	// concatenate all buckets into arr[]
	for (int i = 0; i < buckets.capacity(); i++) {
		for (int j = 0; j < buckets[i]; j++) {
				arr[b_count++] = i;
				// b_count++;
		}
	}
}

/* ****** pthread ****** */
void *bucketSort_bar_pthread(void *args) {
	int tid = atomicTID++;
	int low = ((tid - 1) * arraysize) / NUM_THREADS;
	int high = ((tid * arraysize) / NUM_THREADS) - 1;
	bucketSort_bar_pthread_fn(low, high, tid, args);
	return 0;
}
void bucketSort_bar_pthread_fn(int low, int high, int tid, void *args) {
	int max_value = 0;

	// find max key value in arr
	for (int i = low; i <= high; i++)
		if (arr[i] > max_value) max_value = arr[i];

	// create n empty local buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	pthread_barrier_wait(&bar);
	// put array elements in different buckets
	for (int i = low; i <= high; i++)
		buckets[arr[i]]++;

	pthread_barrier_wait(&bar);
	// concatenate all buckets into arr[]
	for (int i =  0; i < buckets.capacity(); i++) {
		for (int j =  0; j < buckets[i]; j++) {
				arr[b_count++] = i;
		}
	}
	pthread_barrier_wait(&bar);
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
void *bucketSort_TAS(void *args) {
	int tid = atomicTID++;
	int low = ((tid - 1) * arraysize) / NUM_THREADS;
	int high = ((tid * arraysize) / NUM_THREADS) - 1;
	bucketSort_TAS_fn(low, high, tid, args);
	return 0;
}
void bucketSort_TAS_fn(int low, int high, int tid, void *args) {
	int max_value = 0;

	// find max key value in arr
	for (int i = low; i <= high; i++)
		if (arr[i] > max_value) max_value = arr[i];

	// create n empty local buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	// put array elements in different buckets
	for (int i = low; i <= high; i++)
		buckets[arr[i]]++;

	tas_lock();
	// concatenate all buckets into arr[]
	for (int i =  0; i < buckets.capacity(); i++) {
		for (int j =  0; j < buckets[i]; j++) {
				arr[b_count++] = i;
				tas_unlock();
				tas_lock();
		}
	}
	tas_unlock();
}

/* ****** ttas ****** */
void ttas_lock() {
	while(tas_flag.load(SEQ_CST) == true
				|| tas() == false) {}
}
void *bucketSort_TTAS(void *args) {
	int tid = atomicTID++;
	int low = ((tid - 1) * arraysize) / NUM_THREADS;
	int high = ((tid * arraysize) / NUM_THREADS) - 1;
	bucketSort_TTAS_fn(low, high, tid, args);
	return 0;
}
void bucketSort_TTAS_fn(int low, int high, int tid, void *args) {
	int max_value = 0;

	// find max key value in arr
	for (int i = low; i <= high; i++)
		if (arr[i] > max_value) max_value = arr[i];

	// create n empty local buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	// put array elements in different buckets
	for (int i = low; i <= high; i++)
		buckets[arr[i]]++;

	ttas_lock();
	// concatenate all buckets into arr[]
	for (int i =  0; i < buckets.capacity(); i++) {
		for (int j =  0; j < buckets[i]; j++) {
				arr[b_count++] = i;
				tas_unlock();
				ttas_lock();
		}
	}
	tas_unlock();
}


/* ****** ticket ****** */
void ticket_lock() {
	// fai is fetch and increment, but not in C++?
	int num = atomic_fetch_add(&next_num, 1);
	while(now_serving.load(SEQ_CST) != num);
}
void ticket_unlock() {
	atomic_fetch_add(&now_serving, 1);
}
void *bucketSort_ticket_lock(void *args) {
	int tid = atomicTID++;
	int low = ((tid - 1) * arraysize) / NUM_THREADS;
	int high = ((tid * arraysize) / NUM_THREADS) - 1;
	bucketSort_ticket_lock_fn(low, high, tid, args);
	return 0;
}
void bucketSort_ticket_lock_fn(int low, int high, int tid, void *args) {
	int max_value = 0;

	// find max key value in arr
	for (int i = low; i <= high; i++)
		if (arr[i] > max_value) max_value = arr[i];

	// create n empty local buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	// put array elements in different buckets
	for (int i = low; i <= high; i++)
		buckets[arr[i]]++;

	ticket_lock();
	// concatenate all buckets into arr[]
	for (int i =  0; i < buckets.capacity(); i++) {
		for (int j =  0; j < buckets[i]; j++) {
				arr[b_count++] = i;
				ticket_unlock();
    		ticket_lock();
		}
	}
	ticket_unlock();
}

/* ****** pthread ****** */
void *bucketSort_lock_pthread(void *args) {
	int tid = atomicTID++;
	int low = ((tid - 1) * arraysize) / NUM_THREADS;
	int high = ((tid * arraysize) / NUM_THREADS) - 1;
	bucketSort_lock_pthread_fn(low, high, tid, args);
	return 0;
}
void bucketSort_lock_pthread_fn(int low, int high, int tid, void *args) {
	int max_value = 0;

	// find max key value in arr
	for (int i = low; i <= high; i++)
		if (arr[i] > max_value) max_value = arr[i];

	// create n empty local buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	// put array elements in different buckets
	for (int i = low; i <= high; i++)
		buckets[arr[i]]++;

	pthread_mutex_lock(&mutexLock);
	// concatenate all buckets into arr[]
	for (int i = 0; i < buckets.capacity(); i++) {
		for (int j = 0; j < buckets[i]; j++) {
			arr[b_count++] = i;
		}
	}
	pthread_mutex_unlock(&mutexLock);
}

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ BUCKET SORT ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
void bucketSort(int low, int high) {
	int max_value = 0;

	// find max key value in arr

	for (int i = low; i <= high; i++)
		if (arr[i] > max_value) max_value = arr[i];

	// create n empty local buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	// put array elements in different buckets
	for (int i = low; i <= high; i++)
		buckets[arr[i]]++;

	// concatenate all buckets into arr[]
	for (int i =  0; i < buckets.capacity(); i++) {
		for (int j =  0; j < buckets[i]; j++) {
				arr[b_count++] = i;
		}
	}
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

//check if array sorted correctly - by comparing two arrays (one sorted with sort())
void arrayCheck(int asize, int *a1, int *aCheck2) {
	for (int i = 0; i < asize; i++) {
		if (a1[i] != aCheck2[i]) {
			printf("Array is incorrectly sorted!\nbucket_sort(%i) != sort(%i) at position [%i]\n", a1[i], aCheck2[i], i);
			return;
		}
	}
}
