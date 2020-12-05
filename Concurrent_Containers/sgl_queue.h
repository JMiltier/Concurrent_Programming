#ifdef __APPLE__

#ifndef SGL_QUEUE
#define SGL_QUEUE

using namespace std;

pthread_mutex_t SGL_lock;

void SGL_q_push(queue<int> *q, size_t num){
  pthread_mutex_lock(&SGL_lock);
  q->push(num);
  pthread_mutex_unlock(&SGL_lock);
}

void SGL_q_pop(queue<int> *q){
  pthread_mutex_lock(&SGL_lock);
  if (!q->empty()) q->pop();
  pthread_mutex_unlock(&SGL_lock);
}

#endif // SGL_QUEUE
#endif // __APPLE__