#ifdef __APPLE__

#ifndef ELIM_T_STACK
#define ELIM_T_STACK

#include <iostream>
#include <stack>
#include <pthread.h>
#include <atomic>
#include "pthread_add.h"

#define ACQ memory_order_acquire
#define ACQREL memory_order_acq_rel
#define CAS compare_exchange_weak

using namespace std;

// from treiber_stack, used for elim
class elim_stack {
  public:
    class node {
      public:
        node(int v):val(v) {}
        int val;
        node *down;
    };
  atomic<node *> top;
  bool push(int val);
  int pop();
};

#endif // ELIM_T_STACK
#endif // __APPLE__