#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <algorithm>
#include <string>
#include <vector>
#include "pthread.h" // APPLE
#include "arg_parser.h"

/* algorithms */
#include "ms_queue.h"
#include "sgl_queue.h"
#include "sgl_stack.h"
#include "treiber_stack.h"
#include "baskets_queue.h"
#include "elim_stack.h"

using namespace std;

/* variables */
pthread_t *threads;
size_t NUM_THREADS, algorithm, s, a, b=0;
atomic<bool> test;
string inputFile, line;
vector<int> test_vec, tv1, tv2, tv3;
stack<int> sgl_s, *sgl_stack = &sgl_s;
queue<int> sgl_q, *sgl_queue = &sgl_q;
tstack ts, *tre_stack = &ts;
msqueue ms, *ms_queue = &ms;
queue_t bq, *bas_queue = &bq;
elim_stack es, *es_stack = &es;
elim_stack et, *et_stack = &et;

/* execution time struct */
typedef chrono::high_resolution_clock Clock;

/*************** thread handlers ***************/
// SGL STACK
void *SGL_stack(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  for (int i = start; i < end; ++i) {
	  SGL_s_push(sgl_stack, test_vec[i], test);
  }
  if (sgl_stack->size() == split * NUM_THREADS)
    SGL_s_pop(sgl_stack, test);
	return NULL;
}

// SGL QUEUE
void *SGL_queue(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  for (int i = start; i < end; ++i) {
	  SGL_q_push(sgl_queue, test_vec[i], test);
  }
  if (sgl_queue->size() == split * NUM_THREADS)
    SGL_q_pop(sgl_queue, test);
	return NULL;
}

// TREIBER STACK
void *treiber_stack(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  int fin = 0;
  for (int i = start; i < end; ++i) {
	  tre_stack->push(test_vec[i], test);
    fin = test_vec[i+1];
  }
  if (fin == test_vec[split * NUM_THREADS])
    while (tre_stack->pop(test) != NULL);
	return NULL;
}

// MS QUEUE
void *MS_queue(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  int fin = 0;
  for (int i = start; i < end; ++i) {
	  ms_queue->enqueue(test_vec[i], test);
    fin = test_vec[i+1];
  }
  if (fin == test_vec[split * NUM_THREADS])
    while (ms_queue->dequeue(test));
	return NULL;
}

// BASKETS QUEUE
void *baskets_queue(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  int fin = 0;
  for (int i = start; i < end; ++i) {
	  baskets_enqueue(bas_queue, test_vec[i], test);
    fin = test_vec[i+1];
  }
  if (fin == test_vec[split * NUM_THREADS])
    while(baskets_dequeue(bas_queue, test) != -1);
    // printf("%i ");
	return NULL;
}

// ELIMINATION SGL STACK
void *elim_sgl_stack(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  int fin = 0;
  for (int i = start; i < end; ++i) {
	  es_stack->push(test_vec[i], test);
    fin = test_vec[i+1];
  }
  if (fin == test_vec[split * NUM_THREADS])
    while(es_stack->pop(test));
	return NULL;
}

// ELIMINATION TREIBER STACK
void *elim_t_stack(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  int fin = 0;
  for (int i = start; i < end; ++i) {
	  et_stack->push(test_vec[i], test);
    fin = test_vec[i+1];
  }
  if (fin == test_vec[split * NUM_THREADS])
    while(et_stack->pop(test));
	return NULL;
}

// test functions
void testFuncs() {
  threads = static_cast<pthread_t*>(malloc(NUM_THREADS*sizeof(pthread_t)));
  printf("•••••• SGL Stack Tests ••••••\n");
  auto start_time1 = Clock::now();
  for (int i = 0; i < NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, SGL_stack, (void*)i);
	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);
  auto end_time1 = Clock::now();
	unsigned long time_spent1 = chrono::duration_cast<chrono::nanoseconds>(end_time1 - start_time1).count();
  printf("\n Time elapsed: %f seconds", time_spent1/1e9);

  printf("\n•••••• SGL Queue Tests ••••••\n");
  auto start_time2 = Clock::now();
  for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, SGL_queue, (void*)i);
	for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);
  auto end_time2 = Clock::now();
	unsigned long time_spent2 = chrono::duration_cast<chrono::nanoseconds>(end_time2 - start_time2).count();
  printf("\n Time elapsed: %f seconds", time_spent2/1e9);

  printf("\n•••••• Treiber Stack Tests ••••••\n");
  auto start_time3 = Clock::now();
  for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, treiber_stack, (void*)i);
	for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);
  auto end_time3 = Clock::now();
	unsigned long time_spent3 = chrono::duration_cast<chrono::nanoseconds>(end_time3 - start_time3).count();
  printf("\n Time elapsed: %f seconds", time_spent3/1e9);

  printf("\n•••••• Michael And Scott Queue Tests ••••••\n");
  auto start_time4 = Clock::now();
  for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, MS_queue, (void*)i);
	for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);
  auto end_time4 = Clock::now();
	unsigned long time_spent4 = chrono::duration_cast<chrono::nanoseconds>(end_time4 - start_time4).count();
  printf("\n Time elapsed: %f seconds", time_spent4/1e9);

  printf("\n•••••• Baskets Queue Tests ••••••\n");
  auto start_time5 = Clock::now();
  init_queue(bas_queue);
  for (size_t i = 0; i < NUM_THREADS; i++)
    pthread_create(&threads[i], NULL, baskets_queue, (void*)i);
  for (size_t i = 0; i < NUM_THREADS; i++)
    pthread_join(threads[i], NULL);
  auto end_time5 = Clock::now();
	unsigned long time_spent5 = chrono::duration_cast<chrono::nanoseconds>(end_time5 - start_time5).count();
  printf("\n Time elapsed: %f seconds", time_spent5/1e9);

  printf("\n•••••• Elimination SGL Stack Tests ••••••\n");
  auto start_time6 = Clock::now();
  for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, elim_sgl_stack, (void*)i);
	for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);
  auto end_time6 = Clock::now();
	unsigned long time_spent6 = chrono::duration_cast<chrono::nanoseconds>(end_time6 - start_time6).count();
  printf("\n Time elapsed: %f seconds", time_spent6/1e9);

  printf("\n•••••• Elimination Treiber Stack Tests ••••••\n");
  auto start_time7 = Clock::now();
  for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, elim_t_stack, (void*)i);
	for (size_t i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);
  auto end_time7 = Clock::now();
	unsigned long time_spent7 = chrono::duration_cast<chrono::nanoseconds>(end_time7 - start_time7).count();
  printf("\n Time elapsed: %f seconds", time_spent7/1e9);
}

void printVec(vector<int> t) {
  printf("With Nums: { ");
  for (int ii = 0; ii < t.size(); ii++) printf("%i ", t[ii]);
  printf("}\n");
}

// test runs
void runTests() {
	printf("================ STARTING TEST CASES ================\n");
	printf("|     LEGEND:    + push/enqueue    - pop/dequeue    |\n");
	printf("=====================================================\n");
	printf("\n---------------- Testing with 1 Thread ----------------\n");
  NUM_THREADS = 1;
  tv1 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  printVec(tv1);
  test_vec = tv1;
  testFuncs();
  printf("\n\n---------------- Testing with 3 Threads ----------------\n");
  NUM_THREADS = 3;
  tv2 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  printVec(tv2);
  test_vec = tv2;
  testFuncs();
  printf("\n\n---------------- Testing with 5 Threads ----------------\n");
  NUM_THREADS = 5;
  tv3 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  printVec(tv3);
  test_vec = tv3;
  testFuncs();
  printf("\n\n================ END TEST CASES ================\n");
  return;
}