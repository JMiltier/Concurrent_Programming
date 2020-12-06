#ifdef __APPLE__

#ifndef TREIBER_STACK
#define TREIBER_STACK

#include <iostream>
#include <atomic>

#define ACQ memory_order_acquire
#define ACQREL memory_order_acq_rel
#define CAS compare_exchange_weak

using namespace std;

// from lecture 30 slides
class treiber_stack {
  public:
    class node {
      public:
        node(int v):val(v) {}
        int val;
        node *down;
    };
  atomic<node *> top;
  void push(int val);
  int pop();
};

void treiber_stack::push(int val) {
  node *n = new node(val);
  node *t;
  do{
    t = top.load(ACQ);
    n->down = t;
  }while (!top.CAS(t,n,ACQREL));
}

int treiber_stack::pop() {
  node *t, *n;
  int v;
  do{
    t = top.load(ACQ);
    if (t==NULL) {return NULL;}
    n = t->down;
    v = t->val;
  }while (!top.CAS(t,n,ACQREL));
  return v;
}

#endif // TREIBER_STACK
#endif // __APPLE__