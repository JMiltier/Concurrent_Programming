#ifdef __APPLE__

#ifndef SGL_QUEUE
#define SGL_QUEUE

#include <iostream>
#include <queue>

using namespace std;

pthread_mutex_t SGL_queue_lock;

void SGL_q_push(queue<int> *q, size_t num){
  pthread_mutex_lock(&SGL_queue_lock);
  q->push(num);
  pthread_mutex_unlock(&SGL_queue_lock);
}

void SGL_q_pop(queue<int> *q){
  pthread_mutex_lock(&SGL_queue_lock);
  if (!q->empty()) q->pop();
  pthread_mutex_unlock(&SGL_queue_lock);
}

#endif // SGL_QUEUE
#endif // __APPLE__