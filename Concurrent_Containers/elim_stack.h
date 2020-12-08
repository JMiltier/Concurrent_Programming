#ifdef __APPLE__

#ifndef ELIM_SGL_STACK
#define ELIM_SGL_STACK

#include <iostream>
#include <stack>
#include <pthread.h>
#include <atomic>
#include "pthread_add.h"
#include "sgl_stack.h"
#include "treiber_stack.h"

#define ACQ memory_order_acquire
#define ACQREL memory_order_acq_rel
#define CAS __sync_val_compare_and_swap

using namespace std;

struct Node {
  Node *pnext;
  void *pdata;
};

// reimplemented from treiber_stack.h
class elim_stack {
  public:
    class node {
      public:
        node(int v):val(v) {}
        int val;
        Node *down;
    };
  atomic<Node *> top, next;
  bool push(int val);
  int pop();
};

void **location;
int *collision;

struct ThreadInfo {
  size_t *id;
  // char op; // operation of thread
  elim_stack cell;
  // int spin; // amt of delay time for thread to collide
};

void StackOp(size_t *i) {
  if (TryPerformStackOpPush(i)==false)
    LesOP(i);
  return;
}

bool TryPerformStackOpPush(size_t *i, elim_stack *s) {
  ThreadInfo *ti;
  ti->id = i;
  Node *phead, *pnext;
  phead = s->top;
  if(CAS(s->top, phead, &ti->cell))
    return true;
  else return false;
}

void LesOP(ThreadInfo *p) {
  while(true) {
    thread[mypid] = p;
    pos = GetPosition(p)
  }
}

void TryCollisionPush(int tid) {
  if(CAS(&thread[tid], *tid, *tid))
}

void TryCollisionPop(int tid){

}

bool elim_stack::push(int val) {

}

int elim_stack::pop() {

}

#endif // ELIM_SGL_STACK
#endif // __APPLE__

/* Paper notes
 * A push followed by a pop does not change a data structure's state
 * If the two threads can exchange these values without having to touch
 * a centralized structure, they have 'eliminated' the push->pop effect.
 *
 * The idea: use a single elimination array as a backoff scheme on a
 * shared lock-free stack. If the thread fails on the stack, it attempts
 * to eliminate on the array, and if they fail at elimination, they attempt
 * to access the stack again (and so on).
 *
 * Global arrays:
 * * location[1..n] has an element per thread, and pointer location to thread
 * * collision[1..size] holds ids of the threads trying to collide
 *
 * Order of eliminate stack
 * 1st: tries to perform operation on central stack
 * 2nd: if that fails, it goes through the collision layer
 * * 2a: writes thread info to location array
 * * 2b: chooses random location in collision array
 * * 2c: read random location and store it, while trying to write own id
 * * 2d: will keep trying 2c until successful
 *
*/