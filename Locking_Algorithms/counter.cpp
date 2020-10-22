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
#include "cnt_arg_parser.h"   // help with parsing data

using namespace std;

/* initialization of variables */
int NUM_THREADS = 0;
int NUM_ITERATIONS = 0;
int cntr = 0;
int numberOver = 0;
pthread_barrier_t bar;
pthread_mutex_t mutexLock;

/* struct for timer , tv_sec and tv_nsec */
struct timespec time_start, time_end;

/* time output calculation and display */
void time_display(timespec ts, timespec te) {
  // unsigned int 4,294,967,295, which is only 4.3 seconds
  // unsigned long, plan on never running out (over 5 centuries)
  unsigned long time_spent = (te.tv_sec - ts.tv_sec)*1e9 +
                             (te.tv_nsec - ts.tv_nsec);
  printf("Time elapsed is %lu nanoseconds\n", time_spent);
  printf("                %f seconds\n", time_spent/1e9);
}


/* Main Function */
int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	string bar = args_parsed.bar;
	string outputFile = args_parsed.outputFile;
	string lock = args_parsed.lock;
	NUM_THREADS = args_parsed.NUM_THREADS;
  NUM_ITERATIONS = args_parsed.NUM_ITERATIONS;

  // printf("All the data we want:\n%i\n%i\n%s\n%s\n%s\n", NUM_THREADS, NUM_ITERATIONS, bar.c_str(), lock.c_str(), outputFile.c_str());


  // execution time
  clock_gettime(CLOCK_REALTIME, &time_start);
  /*!! do something cool */
  clock_gettime(CLOCK_REALTIME, &time_end);
  time_display(time_start, time_end);
}

/* *************** BAR FUNCTIONS *************** */
// sense
// pthread

/* *************** LOCK FUNCTIONS *************** */
// tas
// ttas
// ticket
atomic_int next_num (0), serving (0), atomicTID (0);

void ticket_lock() {
  int num = atomic_fetch_add(&next_num, 1);
  while(serving.load() != num) {
    if (cntr > NUM_ITERATIONS) {
      numberOver = 1;
      break;
    }
  }
}

void ticket_unlock() {
  atomic_fetch_add(&serving, 1);
}

void *counter_ticket_lock(void *filler) {
  ticket_lock();
  while(cntr < NUM_ITERATIONS) {
    if (!numberOver) cntr++;
    ticket_unlock();
    ticket_lock();
  }
  ticket_unlock();
}
// pthread