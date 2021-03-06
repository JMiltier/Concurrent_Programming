#ifdef __APPLE__

#ifndef SGL_STACK
#define SGL_STACK

#include <iostream>
#include <stack>
#include <pthread.h>
#include "pthread_add.h"

using namespace std;

pthread_mutex_t SGL_stack_lock;

void SGL_s_push(stack<int> *stack, size_t num, bool test){
  pthread_mutex_lock(&SGL_stack_lock);
  stack->push(num);
  // for testing
  if(test) printf("+%i ", stack->top());
  pthread_mutex_unlock(&SGL_stack_lock);
}

void SGL_s_pop(stack<int> *stack, bool test){
  pthread_mutex_lock(&SGL_stack_lock);
  while (!stack->empty()) {
    // for testing
    if(test) printf("-%i ", stack->top());
    stack->pop();
  }
  pthread_mutex_unlock(&SGL_stack_lock);
}

#endif // SGL_STACK
#endif // __APPLE__