#ifdef __APPLE__

#ifndef MS_QUEUE
#define MS_QUEUE

#include <iostream>
#include <atomic>

#define CAS compare_exchange_weak
#define DUMMY 0

using namespace std;

// implemented from lecture 31 slides
class msqueue {
  public:
    class node {
      public:
        node(int v):val(v){}
        int val;
        atomic<node*> next;
    };
    atomic<node*> head, tail;
    msqueue();
    void enqueue(int val, bool test);
    int dequeue(bool test);
};

msqueue::msqueue() {
  node *dummy = new node(DUMMY);
  head.store(dummy);
  tail.store(dummy);
}

void msqueue::enqueue(int value, bool test) {
  node *t, *e, *n, *dummy = NULL;
  n = new node(value);
  while(true) {
    t = tail.load();
    e = t->next.load();
    if(t == tail.load()) {
      if(e == NULL && t->next.CAS(dummy,n)) break;
      else if(e != NULL) tail.CAS(t,e);
    }
  }
  if(test) printf("+%i ", n->val);
  tail.CAS(t,n);
}

int msqueue::dequeue(bool test) {
  node *t, *h, *n;
  while(true) {
    h = head.load();
    t = tail.load();
    n = h->next.load();
    if(h == head.load()){
      if(h == t){
        if(n == NULL) return NULL;
        else {tail.CAS(t,n);}
      } else {
        int ret = n->val;
        if(head.CAS(h,n)) {
          if(test) printf("-%i ", ret);
          return ret;
        }
      }
    }
  }
}

#endif // MS_QUEUE
#endif // __APPLE__