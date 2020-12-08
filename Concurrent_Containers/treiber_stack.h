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
// does not include changes for ABA and reclamation races
class tstack {
  public:
    class node {
      public:
        node(int v):val(v) {}
        int val;
        node *down;
    };
  atomic<node *> top;
  void push(int val, bool test);
  int pop(bool test);
};

void tstack::push(int value, bool test) {
  node *n = new node(value);
  node *t;
  do{
    t = top.load(ACQ);
    n->down = t;
  }while (!top.CAS(t,n,ACQREL));
  if(test) printf("+%i ", n->val);
}

int tstack::pop(bool test) {
  node *t, *n;
  int v;
  do{
    t = top.load(ACQ);
    if (t==NULL) {return NULL;}
    n = t->down;
    v = t->val;
  }while (!top.CAS(t,n,ACQREL));
  if(test) printf("-%i ", v);
  return v;
}

#endif // TREIBER_STACK
#endif // __APPLE__