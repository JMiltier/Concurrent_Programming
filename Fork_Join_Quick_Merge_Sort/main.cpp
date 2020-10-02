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
#include "fj_mergesort.h"
#include "lk_bucketsort.h"
#include "arg_parser.h"

using namespace std;

pthread_t* threads;
size_t* args;
size_t NUM_THREADS;
pthread_barrier_t bar;

struct timespec time_start, time_end;

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
	if(tid==1) clock_gettime(CLOCK_MONOTONIC,&time_start);
	pthread_barrier_wait(&bar);

	// do something
	// printf("Thread %zu reporting for duty\n",tid);

	pthread_barrier_wait(&bar);
	if(tid==1) clock_gettime(CLOCK_MONOTONIC,&time_end);
	local_cleanup();
	return 0;
}

/* ===================== MAIN ==================== */
int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	string inputFile = args_parsed.inputFile;
	string outputFile = args_parsed.outputFile;
	string algorithm = args_parsed.algorithm;
	NUM_THREADS = args_parsed.NUM_THREADS;
	global_init();

	// create array from input file
	fstream file(inputFile.c_str(), ios_base::in);
	int arraysize = 0, a, b = 0;
	string line;
	while (getline(file, line)) arraysize++;
	int array[arraysize];
	fstream infile(inputFile, ios_base::in);
	while (infile >> a) { array[b] = a; b++; }

	/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ ALGO AND THREADS ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
	if (algorithm =="fjmerge") {
		arraysize = arraysize/NUM_THREADS;
		int ret; size_t i;
		for(i=1; i<NUM_THREADS; i++){
			args[i]=i+1;
			printf("creating thread %zu\n",args[i]);
			ret = pthread_create(&threads[i], NULL, &thread_main, &args[i]);
			if(ret){ printf("ERROR; pthread_create: %d\n", ret); exit(-1); }
			fj_mergeSort(array, 0, arraysize - 1, arraysize);
		}
		i = 1;
		thread_main(&i); // master also calls thread_main

		// join threads
		for(size_t i=1; i<NUM_THREADS; i++){
			ret = pthread_join(threads[i], NULL);
			if(ret){ printf("ERROR; pthread_join: %d\n", ret); exit(-1); }
			printf("joined thread %zu\n",i+1);
		}
	}

	else if (algorithm == "lkbucket") lk_bucketSort(array, arraysize);

	/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ALGO AND THREADS ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */


	/* WRITE SORTED ARRAY TO FILE */
	ofstream outfile;
	outfile.open(outputFile);
	for (int i = 0; i < arraysize; i++) outfile << array[i] << endl;
	outfile.close();

	global_cleanup();

	/* DISPLAY EXECUTION TIME */
	unsigned long long elapsed_ns;
	elapsed_ns = (time_end.tv_sec-time_start.tv_sec)*1000000000 + (time_end.tv_nsec-time_start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}