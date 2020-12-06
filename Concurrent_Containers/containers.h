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
// #include <stack> // already included in other header file
// #include <queue> // already included in other header file
#include "pthread.h" // APPLE
#include "arg_parser.h"

/* algorithms */
#include "ms_stack.h"
#include "sgl_queue.h"
#include "sgl_stack.h"
#include "treiber_stack.h"

using namespace std;

extern pthread_mutex_t SGL_stack_lock;

/* variables */
pthread_t *threads;
size_t NUM_THREADS, algorithm, s, a, b=0;
string inputFile, line;
vector<int> test_vec;
stack<int> sgl_s, *sgl_stack = &sgl_s;
queue<int> sgl_q, *sgl_queue = &sgl_q;

/* execution time struct */
typedef chrono::high_resolution_clock Clock;

/* thread handlers */
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

/* thread handlers */
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