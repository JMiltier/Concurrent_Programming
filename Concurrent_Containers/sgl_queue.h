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
  printf("Queue queue: %i\n", q->back());
  pthread_mutex_unlock(&SGL_queue_lock);
}

int SGL_q_pop(queue<int> *q){
  pthread_mutex_lock(&SGL_queue_lock);
  while (!q->empty()) {
    printf("Queue dequeue: %i\n", q->front());
    q->pop();
  }
  pthread_mutex_unlock(&SGL_queue_lock);
}

#endif // SGL_QUEUE
#endif // __APPLE__