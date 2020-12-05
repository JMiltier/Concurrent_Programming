#ifdef __APPLE__

#ifndef SGL_STACK
#define SGL_STACK

using namespace std;

pthread_mutex_t SGL_lock;

void SGL_s_push(stack<int> *s, size_t num){
  pthread_mutex_lock(&SGL_lock);
  s->push(num);
  pthread_mutex_unlock(&SGL_lock);
}

void SGL_s_pop(stack<int> *s){
  pthread_mutex_lock(&SGL_lock);
  if (!s->empty()) s->pop();
  pthread_mutex_unlock(&SGL_lock);
}

#endif // SGL_STACK
#endif // __APPLE__