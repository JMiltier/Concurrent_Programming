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
    void enqueue(int val);
    int dequeue();
};

msqueue::msqueue() {
  node *dummy = new node(DUMMY);
  head.store(dummy);
  tail.store(dummy);
}

void msqueue::enqueue(int val) {
  node *t, *e, *n, *dummy = NULL;
  n = new node(val);
  while(true) {
    t = tail.load();
    e = t->next.load();
    if(t == tail.load()) {
      if(e == NULL && t->next.CAS(dummy,n)){break;}
      else if(e != NULL){tail.CAS(t,e);}
    }
  }
  tail.CAS(t,n);
}

int msqueue::dequeue() {
  node *t, *h, *n;
  while(true) {
    h = head.load();
    t = tail.load();
    n = h->next.load();
    if(h == head.load()){
      if(h == t){
        if(n == NULL) {return NULL;}
        else {tail.CAS(t,n);}
      } else {
        int ret = n->val;
        if(head.CAS(h,n)){return ret;}
      }
    }
  }
}

#endif // MS_QUEUE
#endif // __APPLE__