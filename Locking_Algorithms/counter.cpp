/*
 * counter.cpp
 * program launches 'NUM_THREADS' and each increments the counter 'NUM_ITERATIONS' times
 * the counter is synchronized using either the 'bar' or 'lock' argument
 * keeps track of time taken to increment the counter to its total (of program execution)
 * * printed to standard out in nanoseconds, then showing final counter value to file
 */

#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <time.h>         // for timing execution (timespec), clock_t, clock()
#include "pthread_add.h"  // for when running on macOS
#include "lk_bucketsort.h"
#include "arg_parser.h"   // help with parsing data

using namespace std;

/* initialization of variables */
int NUM_THREADS = 0;
int NUM_OPERATIONS = 0;

/* struct for timer , tv_sec and tv_nsec */
struct timespec time_start, time_end;

/* time output calculation and display */
void time_display(timespec ts, timespec te) {
  // unsigned int 4,294,967,295, which is only 4.3 seconds
  // unsigned long, plan on never running out (over 5 centuries)
  unsigned long time_spent = (te.tv_sec - ts.tv_sec)*1e9 +
                             (te.tv_nsec - ts.tv_nsec);
  printf("Time elapsed is %lu nanoseconds.\n", time_spent);
  printf("                %f seconds.\n", time_spent/1e9);
}

int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	string inputFile = args_parsed.inputFile;
	string outputFile = args_parsed.outputFile;
	string algorithm = args_parsed.algorithm;
	NUM_THREADS = args_parsed.NUM_THREADS;

  clock_gettime(CLOCK_REALTIME, &time_start);
  sleep(3);
  clock_gettime(CLOCK_REALTIME, &time_end);
  time_display(time_start, time_end);
}