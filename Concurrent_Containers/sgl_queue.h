#ifdef __APPLE__

#ifndef SGL_QUEUE
#define SGL_QUEUE

#include <iostream>
#include <queue>

using namespace std;

pthread_mutex_t SGL_queue_lock;

void SGL_q_push(queue<int> *queue, size_t num, int test){
  pthread_mutex_lock(&SGL_queue_lock);
  queue->push(num);
  if(test == -1) {
    printf("%i ", queue->back());
    if(queue->size() == 15)
      printf("\n Dequeued: ");
  }
  pthread_mutex_unlock(&SGL_queue_lock);
}

int SGL_q_pop(queue<int> *queue, int test){
  pthread_mutex_lock(&SGL_queue_lock);
  while (!queue->empty()) {
    if(test == -1) printf("%i ", queue->front());
    queue->pop();
  }
  pthread_mutex_unlock(&SGL_queue_lock);
}

#endif // SGL_QUEUE
#endif // __APPLE__