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
#include <atomic>
#include <mutex>
#include <cmath>
#include <thread>
#include <fstream>
#include <time.h>         // for timing execution (timespec), clock_t, clock()
#include "pthread_add.h"  // for when running on macOS
#include "cnt_arg_parser.h"   // help with parsing data

using namespace std;

#define SEQ_CST memory_order_seq_cst
#define RELAXED memory_order_relaxed

/* initialization */
size_t NUM_THREADS = 0;
int NUM_ITERATIONS = 0;
int COUNTER = 0;
int numberOver = 0;
pthread_barrier_t bar;
pthread_mutex_t mutexLock;
atomic<int> next_num (0), now_serving (0), tid (0);
atomic<int> sense (0), cnt (0);
atomic<bool> tas_flag (0);

/* execution time struct and function */
typedef chrono::high_resolution_clock Clock;

/* bar functions */
void sense_wait();
void *counter_sense(void *);
void *counter_bar_pthread(void *);

/* lock functions */
bool tas();
void tas_lock();
void tas_unlock();
void *counter_TAS(void *);
void ttas_lock();
void *counter_TTAS(void *);
void ticket_lock();
void ticket_unlock();
void *counter_ticket_lock(void *);
void *counter_lock_pthread(void *);


/* *************** Main Function *************** */
int main(int argc, const char* argv[]){
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
  printf("Counter is: %i\n", COUNTER);

  // write to file
  FILE * fp;
  fp = fopen(outputFile.c_str(), "w");
  if (fp == NULL) return -1;
  fprintf(fp, "%d\n", COUNTER);
  fclose(fp);

  return 0;
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

void *counter_sense(void *) {
  int tid = tid++;
  for(int i = 0; i < NUM_ITERATIONS; ++i) {
    COUNTER++;
    sense_wait();
  }
  return NULL;
}

/* ****** pthread ****** */
void *counter_bar_pthread(void *) {
  int tid = tid++;
  pthread_barrier_wait(&bar);
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    COUNTER++;
    pthread_barrier_wait(&bar);
  }
  return NULL;
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

void *counter_TAS(void *) {
  for(int i = 0; i < NUM_ITERATIONS; ++i) {
    tas_lock();
    COUNTER++;
    tas_unlock();
  }
  return NULL;
}

/* ****** ttas ****** */
void ttas_lock() {
  while(tas_flag.load(SEQ_CST) == true
        || tas() == false) {}
}

void *counter_TTAS(void *) {
  ttas_lock();
  for(int i = 0; i < NUM_ITERATIONS; ++i) {
    COUNTER++;
    tas_unlock();
    ttas_lock();
  }
  tas_unlock();
  return NULL;
}

/* ****** ticket ****** */
void ticket_lock() {
  // fai is fetch and increment, but not in C++?
  int num = atomic_fetch_add(&next_num, 1);
  while(now_serving.load(SEQ_CST) != num) {
    if (COUNTER > NUM_ITERATIONS*NUM_THREADS) {
      numberOver = 1;
      break;
    }
  }
}

void ticket_unlock() {
  // fai is fetch and increment, but not in C++?
  atomic_fetch_add(&now_serving, 1);
}

void *counter_ticket_lock(void *) {
  ticket_lock();
  for(int i = 0; i < NUM_ITERATIONS; ++i) {
    if (!numberOver) COUNTER++;
    ticket_unlock();
    ticket_lock();
  }
  ticket_unlock();
  return NULL;
}

/* ****** pthread ****** */
void *counter_lock_pthread(void *) {
  int tid = tid++;
  for(int i = 0; i < NUM_ITERATIONS; ++i) {
    while (COUNTER % (NUM_THREADS - 1) != tid) {
      if (COUNTER >= NUM_ITERATIONS) break;
    }
    pthread_mutex_lock(&mutexLock);
    COUNTER++;
    pthread_mutex_unlock(&mutexLock);
  }
  return NULL;
}