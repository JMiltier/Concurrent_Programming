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

/* =============================================== */
/* ===================== MAIN ==================== */
int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	string inputFile = args_parsed.inputFile;
	string outputFile = args_parsed.outputFile;

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
	while (infile >> a) { arr[b] = a; b++; }

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
}

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ MERGE SORT ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
// Merge sort, from lab0
void merge(int *arr, int low, int high, int mid, int len) {
    int i, j, k, c[len];
    i = low;
    k = low;
    j = mid + 1;
    while (i <= mid && j <= high) {
        if (arr[i] < arr[j]) {
            c[k] = arr[i];
            k++;
            i++;
        }
        else  {
            c[k] = arr[j];
            k++;
            j++;
        }
    }
    while (i <= mid) {
        c[k] = arr[i];
        k++;
        i++;
    }
    while (j <= high) {
        c[k] = arr[j];
        k++;
        j++;
    }

    for (i = low; i < k; i++)
        arr[i] = c[i];
}

void mergeSort(int *arr, int low, int high, int len) {
    if (low < high){
        //divide the array at mid and sort independently using merge sort
        int mid=(low+high)/2;
        mergeSort(arr,low,mid,len);
        mergeSort(arr,mid+1,high,len);
        //merge or conquer sorted arrays
        merge(arr,low,high,mid,len);
    }
}
/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END MERGE SORT ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */