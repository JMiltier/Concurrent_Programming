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

/* initialization */
int NUM_THREADS = 0;
int NUM_ITERATIONS = 0;
int cntr = 0;
int numberOver = 0;
bool flagOut = false;
pthread_barrier_t bar;
pthread_mutex_t mutexLock;
atomic_int next_num (0), serving (0), atomicTID (0);
atomic_int sense (0), cnt (0);
atomic_bool tas_flag (false);

/* execution time struct and function */
struct timespec time_start, time_end;
void time_display(timespec ts, timespec te);

/* bar functions */
void sense_wait();
void *counter_sense(void *filler);
void *counter_bar_pthread(void *filler);

/* lock functions */
bool tas();
void tas_lock();
void tas_unlock();
void *counter_TSL(void *filler);
void ttas_lock();
void *counter_TTSL(void *filler);
void ticket_lock();
void ticket_unlock();
void *counter_ticket_lock(void *filler);
void *counter_lock_pthread(void *filler);


/* *************** Main Function *************** */
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


/* *************** time output calculation and display *************** */
void time_display(timespec ts, timespec te) {
  // unsigned int 4,294,967,295, which is only 4.3 seconds
  // unsigned long, plan on never running out (over 5 centuries)
  unsigned long time_spent = (te.tv_sec - ts.tv_sec)*1e9 +
                             (te.tv_nsec - ts.tv_nsec);
  printf("Time elapsed is %lu nanoseconds\n", time_spent);
  printf("                %f seconds\n", time_spent/1e9);
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

void *counter_sense(void *filler) {
  int tid = atomicTID++;
  while (cntr < NUM_ITERATIONS) {
    cntr++;
    sense_wait();
  }
}

/* ****** pthread ****** */
void *counter_bar_pthread(void *filler) {
  int tid = atomicTID++;
  pthread_barrier_wait(&bar);
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    if (i % NUM_THREADS == tid) cntr++;
    pthread_barrier_wait(&bar);
  }
}

/* *************** LOCK FUNCTIONS *************** */
/* ****** tas ****** */
bool tas() {
  if (tas_flag == false) tas_flag = true;
  return tas_flag;
}

void tas_lock() {
  while(!tas());
}

void tas_unlock() {
  tas_flag.store(false, memory_order_seq_cst);
}

void *counter_TSL(void *filler) {
  tas_lock();
  while(cntr < NUM_ITERATIONS) {
    cntr++;
    tas_unlock();
    tas_lock();
  }
  tas_unlock();
}

/* ****** ttas ****** */
void ttas_lock() {
  while(tas_flag.load() || !tas());
}

void *counter_TTSL(void *filler) {
  ttas_lock();
  while(cntr < NUM_ITERATIONS) {
    cntr++;
    tas_unlock();
    ttas_lock();
  }
  tas_unlock();
}

/* ****** ticket ****** */
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

/* ****** pthread ****** */
void *counter_lock_pthread(void *filler) {
  int tid = atomicTID++;
  while (cntr < NUM_ITERATIONS) {
    while (cntr % (NUM_THREADS - 1) != tid) {
      if (cntr >= NUM_ITERATIONS) break;
    }
    pthread_mutex_lock(&mutexLock);
    cntr++;
    pthread_mutex_unlock(&mutexLock);
  }
}