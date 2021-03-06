#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <time.h>
#include "pthread_add.h" // added for Mac use
#include "arg_parser.h"

using namespace std;

pthread_t* threads;
size_t* args;
size_t NUM_THREADS;
pthread_barrier_t bar;
int arrsize, arr[100000000];
atomic<int> b_count (0);
mutex b_lock; 					// lock for bucket sorting

/* execution time struct */
typedef chrono::high_resolution_clock Clock;

/* merge sort functions */
void merge(int low, int high, int mid);
void mergeSort(int low, int high);
void* fj_mergeSort(void* args);
/* bucket sort functions */
void bucketSort(int low, int high);
void* lk_bucketSort(void* args);
/* array sorting checking */
void arrayCheck(int arraysize, int *array1, int *array2) ;


/* =============================================== */
/* ===================== MAIN ==================== */
int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	string inputFile = args_parsed.inputFile;
	string outputFile = args_parsed.outputFile;
	string algorithm = args_parsed.algorithm;
	NUM_THREADS = args_parsed.NUM_THREADS;

	// init
	threads = static_cast<pthread_t*>(malloc(NUM_THREADS*sizeof(pthread_t)));
	args = static_cast<size_t*>(malloc(NUM_THREADS*sizeof(size_t)));
	pthread_barrier_init(&bar, NULL, NUM_THREADS);

	// create array from input file
	fstream file(inputFile.c_str(), ios_base::in);
	int a, b = 0;
	arrsize = 0;
	string line;
	while (getline(file, line)) arrsize++;
	// arr[arrsize];
	fstream infile(inputFile, ios_base::in);
	int arrCheck[arrsize];
	while (infile >> a) {
		arr[b] = a;
		arrCheck[b] = arr[b];
		b++;
	}

	// execution start time
	auto start_time = Clock::now();

	/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ ALGO AND THREADS ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
	if (algorithm =="fjmerge") {
		// if number of threads to use is one, no need to run through
		if (NUM_THREADS == 1) mergeSort(0, arrsize - 1);
		else {
			int ret; size_t i;
			for(i = 0; i < NUM_THREADS; i++){
				args[i] = i;
				printf("creating thread %zu\n",args[i]+1);
				// ret = pthread_create(&threads[i], NULL, &thread_main, &args[i]);
				ret = pthread_create(&threads[i], NULL, &fj_mergeSort, &args[i]);
				if(ret){ printf("ERROR; pthread_create: %d\n", ret); exit(-1); }
			}

			// join threads
			for(i = 0; i < NUM_THREADS; i++){
				ret = pthread_join(threads[i], NULL);
				if(ret){ printf("ERROR; pthread_join: %d\n", ret); exit(-1); }
				printf("joined thread %zu\n",i+1);
			}

			// final merge, since broken up into NUM_THREAD sorted parts/chunks
			// imagining there is a better way to do this
			mergeSort(0, arrsize-1);
		}
	}

	else if (algorithm == "lkbucket"){
		if (NUM_THREADS == 1) bucketSort(0, arrsize - 1);
		else {
			int ret; size_t i;
			for(i = 0; i < NUM_THREADS; i++){
				args[i] = i;
				printf("creating thread %zu\n",args[i]+1);
				// lock_guard<mutex> lock(b_lock);
				ret = pthread_create(&threads[i], NULL, &lk_bucketSort, &args[i]);
				if(ret){ printf("ERROR; pthread_create: %d\n", ret); exit(-1); }
			}

			// join threads
			for(i = 0; i < NUM_THREADS; i++){
				ret = pthread_join(threads[i], NULL);
				if(ret){ printf("ERROR; pthread_join: %d\n", ret); exit(-1); }
				printf("joined thread %zu\n",i+1);
			}

			// final sort since only NUM_THREADS sub-buckets are sorted
			// again, not the best way to do this
			b_count = 0;
			bucketSort(0, arrsize);
		}
	}
	/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ALGO AND THREADS ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

	// execution end time
	auto end_time = Clock::now();
	// calculate and display execution time
	unsigned long time_spent = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
	printf("Time elapsed: %lu nanoseconds\n", time_spent);
	printf("               %f seconds\n", time_spent/1e9);

	/* WRITE SORTED ARRAY TO FILE */
	ofstream outfile;
	outfile.open(outputFile);
	for (int i = 0; i < arrsize; i++) outfile << arr[i] << endl;
	outfile.close();

	// cleanup
	free(threads);
	free(args);
	pthread_barrier_destroy(&bar);

	// ensure array was sorted
	sort(arrCheck, arrCheck + arrsize);
	arrayCheck(arrsize, arr, arrCheck);
}

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ MERGE SORT ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
void merge( int low, int high, int mid) {
	int i, j, k;
	int l_size = mid - low + 1;
	int r_size = high - mid;
	int left[l_size];
	int right[r_size];

	for (i = 0; i < l_size; i++)
		left[i] = arr[i + low];

	for (i = 0; i < r_size; i++)
		right[i] = arr[i + mid + 1];

	i = j = 0; k = low;

	while(i < l_size && j < r_size){
		if (left[i] <= right[j]) arr[k++] = left[i++];
		else arr[k++] = right[j++];
	}

	// copy remaining vals
	while (i < l_size) arr[k++] = left[i++];
	while (j < r_size) arr[k++] = right[j++];
}

void mergeSort(int low, int high) {
    //divide the array at mid
		int mid = low + (high - low) / 2;
    if (low < high){
        // sort independently using merge sort
        // mid=(low+high)/2;
        mergeSort(low, mid);
        mergeSort(mid + 1, high);
        //merge or conquer sorted arrays
        merge(low, high, mid);
    }
}


void* fj_mergeSort(void* args){
	size_t tid = *((size_t*)args);

	int arrsplit = arrsize / NUM_THREADS;
	int low = tid * arrsplit;
	int high = ((tid + 1) * arrsplit) - 1;

	pthread_barrier_wait(&bar);
	int mid = low + (high - low) / 2;
	if (low < high) {
		mergeSort(low, mid);
		mergeSort(mid + 1, high);
		merge(low, mid, high);
	}

	return 0;
}
/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END MERGE SORT ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ BUCKET SORT ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
// Function to sort arr[] from [left to right] bucket sort
void bucketSort(int low, int high) {
	int i, j, max_value = 0;

	// find max key value in arr
	for (i = low; i < high; ++i)
		if (arr[i] > max_value) max_value = arr[i];

	// create n empty local buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	// put array elements in different buckets
	for (i = low; i < high; ++i) // change <= to <
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

	printf("tid: %zu", tid);

	int arrsplit = arrsize / NUM_THREADS;
	int low = tid * arrsplit;
	int high = ((tid + 1) * arrsplit) - 1; // forgot to subtract 1

	lock_guard<mutex> guard(b_lock);
	bucketSort(low, high);

	return 0;
}
/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END BUCKET SORT ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

//check if array sorted correctly - by comparing two sorted arrays
void arrayCheck(int arraysize, int *array1, int *array2) {
  for (int i = 0; i < arraysize; i++) {
    if (array1[i] != array2[i]) {
        cout << "Array is incorrectly sorted!\n" << array1[i] << "!=" << array2[i] << " at position " << i << endl;
        return;
    }
  }
}
