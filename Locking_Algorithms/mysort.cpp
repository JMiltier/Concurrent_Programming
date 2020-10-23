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
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include "pthread_add.h" // for when running on macOS
#include "lk_bucketsort.h"
#include "arg_parser.h"

using namespace std;

pthread_t* threads;
size_t* args;
size_t NUM_THREADS;
pthread_barrier_t bar;
int arraysize, arr[100000], thread_num;

/* execution time struct */
typedef chrono::high_resolution_clock Clock;

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

void local_init(){
	int arr[arraysize];
}
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
	int a, b = 0;
	arraysize = 0;
	string line;
	while (getline(file, line)) arraysize++;
	arr[arraysize];
	fstream infile(inputFile, ios_base::in);
	while (infile >> a) { arr[b] = a; b++; }

	struct arg_params args_parsed = arg_parser(argc, argv);
	int argument = args_parsed.argument;
	string outputFile = args_parsed.outputFile;
	NUM_THREADS = args_parsed.NUM_THREADS;
  NUM_ITERATIONS = args_parsed.NUM_ITERATIONS;
  pthread_t threads[NUM_THREADS];

  // printf("All the data we want:\n%i\n%i\n%i\n%s\n", NUM_THREADS, NUM_ITERATIONS, argument, outputFile.c_str());

  // execution start time
  auto start_time = Clock::now();

  /* argument statement (from parser) is as follows:
   * bar: 1-sense, 2-pthread
   * lock: 3-tas, 4-ttas, 5-ticket, 6-pthread */
  switch (argument) {
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

    // lock tas
    case 3:
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_create(&threads[i], NULL, counter_TAS, (void*)NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_join(threads[i], NULL);
      break;
    // lock ttas
    case 4:
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_create(&threads[i], NULL, counter_TTAS, (void*)NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_join(threads[i], NULL);
      break;
    // lock ticket
    case 5:
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_create(&threads[i], NULL, counter_ticket_lock, (void*)NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_join(threads[i], NULL);
      break;
    // lock pthread
    case 6:
      pthread_mutex_init(&mutexLock, NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_create(&threads[i], NULL, counter_lock_pthread, (void*)NULL);
      for (int i = 0; i < NUM_THREADS; i ++)
        pthread_join(threads[i], NULL);
      pthread_mutex_destroy(&mutexLock);
      break;

    // something didn't match up
    default:
      printf("An error occured in MAIN.");
      exit(-1);
  }

  // execution end time
  auto end_time = Clock::now();
  // unsigned int 4,294,967,295, which is only 4.3 seconds
  // unsigned long, plan on never running out (over 5 centuries)
  unsigned long time_spent = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
  printf("Time elapsed is %lu nanoseconds\n", time_spent);
  printf("                %f seconds\n", time_spent/1e9);

	/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ ALGO AND THREADS ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
	if (algorithm =="fjmerge") {
		if (NUM_THREADS == 1) mergeSort(arr, 0, arraysize - 1);
		else {
			int ret; size_t i;
			for(i=1; i<NUM_THREADS; i++){
				args[i]=i+1;
				printf("creating thread %zu\n",args[i]);
				// ret = pthread_create(&threads[i], NULL, &thread_main, &args[i]);
				ret = pthread_create(&threads[i], NULL, &fj_mergeSort, &args[i]);
				if(ret){ printf("ERROR; pthread_create: %d\n", ret); exit(-1); }
			}
			// i = 1;
			// thread_main(&i); // master also calls thread_main

			// join threads
			for(size_t i=1; i<NUM_THREADS; i++){
				ret = pthread_join(threads[i], NULL);
				if(ret){ printf("ERROR; pthread_join: %d\n", ret); exit(-1); }
				printf("joined thread %zu\n",i+1);
			}
		}
	}

	else if (algorithm == "lkbucket") lk_bucketSort(arr, arraysize);

	/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ALGO AND THREADS ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */


	/* WRITE SORTED ARRAY TO FILE */
	ofstream outfile;
	outfile.open(outputFile);
	for (int i = 0; i < arraysize; i++) outfile << arr[i] << endl;
	outfile.close();

	global_cleanup();

	/* DISPLAY EXECUTION TIME */
	unsigned long long elapsed_ns;
	elapsed_ns = (time_end.tv_sec-time_start.tv_sec)*1000000000 + (time_end.tv_nsec-time_start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}