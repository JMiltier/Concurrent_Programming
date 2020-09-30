#define _POSIX_C_SOURCE 200112L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <string> // string conversion
#include "pthread_add.h"

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

int main(int argc, const char* argv[]){
	string inputFile, outputFile, algorithm;

	// check for name
	if (argc == 2 && string(argv[1]) == "--name") {
		printf("Josh Miltier\n");
		return 0;
  } else if (argc == 7) {
		// continue on to execution
	} else { // catch all
		printf("\nERROR: program options are invalid.\nRefer to the readme for execution instructions.\n\n");
		exit(-1);
	}

		global_init();

 	/* ************* ARG PARSER ************* */
	// txt input file (argv 1)
	inputFile = string(argv[1]);

	// txt output file (argv 4), with syntax checking
	if (string(argv[2]) != "-o") {
		printf("ERROR: incorrect flag for output file.   Expected: '-o'  Actual: %s\n", argv[2]); exit(-1);
	} else outputFile = string( argv[3] );

	// number of threads (argv 6), with syntax checking
	if (string(argv[4]) != "-t") {
		printf("ERROR: incorrect flag for number of threads.   Expected: '-t'  Actual: %s\n", argv[4]); exit(-1);
	} else {
		NUM_THREADS = atoi( argv[5] );
		if (NUM_THREADS > 150) { printf("ERROR; too many threads\n"); exit(-1); }
		else NUM_THREADS = 5;
	}

	// algorithm type <fjmerge> or <lkbucket> (argv 7)
	// fork/join merge sort
	if (string(argv[6]) == "--alg=fjmerge") algorithm = "fkmerge";
	// lock bucket sort
	else if (string(argv[6]) == "--alg=lkbucket") algorithm = "lkbucket";
	// algorithm not available
	else printf("ERROR: algorithm does not exist. Try 'fjmerge' or 'lkbucket'\n");
 	/* *********** END ARG PARSER *********** */

	// launch threads
	int ret; size_t i;
  for(i=1; i<NUM_THREADS; i++){
		args[i]=i+1;
		printf("creating thread %zu\n",args[i]);
		ret = pthread_create(&threads[i], NULL, &thread_main, &args[i]);
		if(ret){
			printf("ERROR; pthread_create: %d\n", ret);
			exit(-1);
		}
	}
	i = 1;
	thread_main(&i); // master also calls thread_main

	// join threads
	for(size_t i=1; i<NUM_THREADS; i++){
		ret = pthread_join(threads[i],NULL);
		if(ret){
			printf("ERROR; pthread_join: %d\n", ret);
			exit(-1);
		}
		printf("joined thread %zu\n",i+1);
	}

	// read from file
	fstream file(inputFile, ios_base::in);
    int arraysize = 0;
    string line;
    while (getline(file, line)) { arraysize++; }
	int array[arraysize], a, b = 0;
	fstream infile(inputFile, ios_base::in);
	while (infile >> a) { array[b] = a; b++; }

	// write to file
	ofstream outfile;
	outfile.open(outputFile);
	for (int i = 0; i < arraysize; i++) {
		outfile << array[i] << endl;
	}
	outfile.close();

	global_cleanup();

	unsigned long long elapsed_ns;
	elapsed_ns = (time_end.tv_sec-time_start.tv_sec)*1000000000 + (time_end.tv_nsec-time_start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}