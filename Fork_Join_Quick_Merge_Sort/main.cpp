#define _POSIX_C_SOURCE 200112L
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include "pthread_add.h" // for when running on macOS

using namespace std;

pthread_t* threads;
size_t* args;
size_t NUM_THREADS;
pthread_barrier_t bar;

struct timespec time_start, time_end;

/* **************************************************************** */
/* *************************v GLOBAL INIT v************************ */
void global_init(){
	threads = static_cast<pthread_t*>(malloc(NUM_THREADS*sizeof(pthread_t)));
	args = static_cast<size_t*>(malloc(NUM_THREADS*sizeof(size_t)));
	pthread_barrier_init(&bar, NULL, NUM_THREADS);
}

void global_cleanup(){
	free(threads);
	free(args);
	pthread_barrier_destroy(&bar);
}

void local_init(){}
void local_cleanup(){}

void* thread_main(void* args){
	size_t tid = *((size_t*)args);
	local_init();
	pthread_barrier_wait(&bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&time_start);
	}
	pthread_barrier_wait(&bar);

	// do something
	printf("Thread %zu reporting for duty\n",tid);

	pthread_barrier_wait(&bar);
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&time_end);
	}
	local_cleanup();

	return 0;
}
/* ***********************^ END GLOBAL INIT ^********************** */
/* **************************************************************** */

/* *************************************************************** */
/* *************************v MERGE SORT v************************ */
void fj_merge(int *arr, int low, int high, int mid, int len) {
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
    for (i = low; i < k; i++)  {
        arr[i] = c[i];
    }
}

void fj_mergeSort(int *arr, int low, int high, int len) {
    int mid;
    if (low < high){
        //divide the array at mid and sort independently using merge sort
        mid=(low+high)/2;
        fj_mergeSort(arr,low,mid,len);
        fj_mergeSort(arr,mid+1,high,len);
        //merge or conquer sorted arrays
        fj_merge(arr,low,high,mid,len);
    }
}
/* ***********************^ END MERGE SORT ^********************** */
/* *************************************************************** */

/* *************************************************************** */
/* *************************v BUCKET SORT v*********************** */
// Function to sort arr[] of size n using bucket sort
void lk_bucketSort(int *arr, int n) {
	int i, j, count = 0, max_value = 0;

	// find max key value in arr
	for (i = 0; i < n; ++i) {
		if (arr[i] > max_value) max_value = arr[i];
	}

	// create n empty buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	// put array elements in different buckets
	for (i = 0; i < n; ++i) {
		buckets[arr[i]]++;
	}

	// concatenate all buckets into arr[]
	for (int i = 0; i < buckets.capacity(); ++i) {
			for (int j = 0; j < buckets[i]; ++j) {
					arr[count] = i;
					count++;
			}
	}
}
/* ***********************^ END BUCKET SORT ^********************* */
/* *************************************************************** */

/* ****************************** MAIN ****************************** */
int main(int argc, const char* argv[]){
	string inputFile, outputFile, algorithm;

	/* *************v ARG CHECKER v************* */
	if (argc == 2 && string(argv[1]) == "--name") { printf("Josh Miltier\n"); return 0; } // check for --name
	else if (argc == 7) {} // continue on to execution
	else { // catch all else
		printf("\nERROR: Program option parameters are invalid.\n"
					"       Expected 6 options, but got %i.\n"
					"       Refer to the README for execution instructions.\n\n", argc-1);
		exit(-1);
	}
	/* *************^ END ARG CHECK ^************* */

	global_init();

	/* ************************************** */
 	/* *************v ARG PARSER v************* */
	// txt input file (argv 1)
	inputFile = string(argv[1]);

	// txt output file (argv 3), with syntax checking
	if (string(argv[2]).rfind("-o", 0)) {
		printf("ERROR: incorrect flag for output file.   Expected: '-o'  Actual: '%s'\n", argv[2]); exit(-1);
	} else outputFile = string(argv[3]);

	// number of threads (argv 5), with syntax checking
	if (!string(argv[4]).rfind("-t", 0)) {
		NUM_THREADS = atoi( argv[5] );
		if (NUM_THREADS > 150) { printf("ERROR; too many threads\n"); exit(-1); }
	} else {
		printf("ERROR: incorrect flag for number of threads.   Expected: '-t'  Actual: '%s'\n", argv[4]); exit(-1);
	}

	// algorithm type <fjmerge> or <lkbucket> (argv 7)
	// fork/join merge sort
	if (string(argv[6]) == "--alg=fjmerge") algorithm = "fjmerge";
	// lock bucket sort
	else if (string(argv[6]) == "--alg=lkbucket") algorithm = "lkbucket";
	// algorithm not available
	else { printf("ERROR: algorithm does not exist. Try 'fjmerge' or 'lkbucket'\n"); exit(-1); }
 	/* ***********^ END ARG PARSER ^*********** */
	/* ************************************** */
	/* ******************************************* */
	/* ****************v THREADS v**************** */
	// int ret; size_t i;
  // for(i=1; i<NUM_THREADS; i++){
	// 	args[i]=i+1;
	// 	printf("creating thread %zu\n",args[i]);
	// 	ret = pthread_create(&threads[i], NULL, &thread_main, &args[i]);
	// 	if(ret){
	// 		printf("ERROR; pthread_create: %d\n", ret);
	// 		exit(-1);
	// 	}
	// }
	// i = 1;
	// thread_main(&i); // master also calls thread_main

	// // join threads
	// for(size_t i=1; i<NUM_THREADS; i++){
	// 	ret = pthread_join(threads[i],NULL);
	// 	if(ret){
	// 		printf("ERROR; pthread_join: %d\n", ret);
	// 		exit(-1);
	// 	}
	// 	printf("joined thread %zu\n",i+1);
	// }
	/* **************^ END THREADS ^************** */
	/* ******************************************* */

	/* ****************************************************** */
	/* **************v FILE AND ALGO HANDLING v************** */
	// read from file
	fstream file(inputFile, ios_base::in);
    int arraysize = 0;
    string line;
    while (getline(file, line)) { arraysize++; }
	int array[arraysize], a, b = 0;
	fstream infile(inputFile, ios_base::in);
	while (infile >> a) { array[b] = a; b++; }

	if (algorithm == "fjmerge") fj_mergeSort(array, 0, arraysize - 1, arraysize);
	else if (algorithm == "lkbucket") lk_bucketSort(array, arraysize);

	// write to file
	ofstream outfile;
	outfile.open(outputFile);
	for (int i = 0; i < arraysize; i++) {
		outfile << array[i] << endl;
	}
	outfile.close();
	/* ************^ END FILE AND ALGO HANDLING ^************ */
	/* ****************************************************** */

	global_cleanup();

	unsigned long long elapsed_ns;
	elapsed_ns = (time_end.tv_sec-time_start.tv_sec)*1000000000 + (time_end.tv_nsec-time_start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}