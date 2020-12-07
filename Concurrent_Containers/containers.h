// #include <iostream> // already in other header file
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
// #include <stack> // already included in other header file
// #include <queue> // already included in other header file
#include "pthread.h" // APPLE
#include "arg_parser.h"

/* algorithms */
#include "ms_queue.h"
#include "sgl_queue.h"
#include "sgl_stack.h"
#include "treiber_stack.h"
#include "baskets_queue.h"
#include "elim_sgl_stack.h"
#include "elim_t_stack.h"

using namespace std;

/* variables */
pthread_t *threads;
size_t NUM_THREADS, algorithm, s, a, b=0;
string inputFile, line;
vector<int> test_vec;
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
	  SGL_s_push(sgl_stack, test_vec[i]);
  }
  if (sgl_stack->size() == split * NUM_THREADS)
    SGL_s_pop(sgl_stack);
	return NULL;
}

// SGL QUEUE
void *SGL_queue(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  for (int i = start; i < end; ++i) {
	  SGL_q_push(sgl_queue, test_vec[i]);
  }
  if (sgl_queue->size() == split * NUM_THREADS)
    SGL_q_pop(sgl_queue);
	return NULL;
}

// TREIBER STACK
void *treiber_stack(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  for (int i = start; i < end; ++i) {
	  tre_stack->push(test_vec[i]);
  }
  tre_stack->pop();
	return NULL;
}

// MS QUEUE
void *MS_queue(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  for (int i = start; i < end; ++i) {
	  ms_queue->enqueue(test_vec[i]);
  }
  ms_queue->dequeue();
	return NULL;
}

// BASKETS QUEUE
void *baskets_queue(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  printf("YOO\n");
  for (int i = start; i < end; ++i) {
    printf("YOO2\n");
	  baskets_enqueue(bas_queue, test_vec[i]);
    printf("YOO2.2\n");
  }
  size_t t = 0;
  printf("YOO3\n");
  while(t != 999) {
    printf("YOO999\n");
    t = baskets_dequeue(bas_queue);
  }

	return NULL;
}

// ELIMINATION SGL STACK
void *elim_sgl_stack(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  for (int i = start; i < end; ++i) {
	  ms_queue->enqueue(test_vec[i]);
  }
  ms_queue->dequeue();
	return NULL;
}

// ELIMINATION TREIBER STACK
void *elim_t_stack(void *i) {
  int tid = (int)(size_t)i;
	size_t split = test_vec.size() / NUM_THREADS;
  size_t start = split * tid;
  size_t end = split * (tid+1);
  for (int i = start; i < end; ++i) {
	  ms_queue->enqueue(test_vec[i]);
  }
  ms_queue->dequeue();
	return NULL;
}