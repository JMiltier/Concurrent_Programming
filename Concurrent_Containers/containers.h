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
stack<int> sgl_s;
stack<int> *sgl_stack = &sgl_s;
queue<int> sgl_q;
queue<int> *sgl_queue = &sgl_q;

/* execution time struct */
typedef chrono::high_resolution_clock Clock;

/* thread handlers */
// void *SGL_stack(void *i);

void *SGL_stack(void *i) {
  printf("called correctly\n");
	size_t num = (s-1) / NUM_THREADS;
  printf("size %i, and num %i\n", s, num);
	SGL_s_push(sgl_stack, test_vec[num]);
	return NULL;
}
