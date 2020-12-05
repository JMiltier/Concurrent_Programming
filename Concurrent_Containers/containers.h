#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <algorithm>
#include <string>
#include <vector>
#include "arg_parser.h"

/* algorithms */
#include "ms_stack.h"
#include "sgl_queue.h"
#include "sgl_stack.h"
#include "treiber_stack.h"

using namespace std;

/* variables */
pthread_t* threads;
size_t NUM_THREADS, algorithm, s, a, b=0;
string inputFile, line;
vector<int> test_vec;

/* execution time struct */
typedef chrono::high_resolution_clock Clock;
