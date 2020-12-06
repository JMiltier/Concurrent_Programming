#ifdef __APPLE__

#ifndef SGL_STACK
#define SGL_STACK

#include <iostream>
#include <stack>
#include <pthread.h>
#include "pthread_add.h"

using namespace std;

pthread_mutex_t SGL_stack_lock;

void SGL_s_push(stack<int> *s, size_t num){
  pthread_mutex_lock(&SGL_stack_lock);
  printf("Pushing %i onto stack\n", num);
  s->push(num);
  printf("Stack: %i\n", s->top());
  pthread_mutex_unlock(&SGL_stack_lock);
}

void SGL_s_pop(stack<int> *s){
  pthread_mutex_lock(&SGL_stack_lock);
  if (!s->empty()) s->pop();
  pthread_mutex_unlock(&SGL_stack_lock);
}

#endif // SGL_STACK
#endif // __APPLE__