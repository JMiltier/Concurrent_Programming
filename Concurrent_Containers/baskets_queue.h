#ifdef __APPLE__

#ifndef BASKETS_QUEUE
#define BASKETS_QUEUE

#include <iostream>
#include <atomic>
#include <unistd.h>
#include <time.h> // for nanosleep()

#define MAX_HOPS 3
#define CAS __sync_val_compare_and_swap // References #16

// from baskets queue reference (p. 405)
struct node_t;
struct pointer_t {
  node_t * ptr;
  bool deleted;
  size_t tag;
};
struct node_t { size_t value; pointer_t next; };
struct queue_t { pointer_t tail; pointer_t head; };

void init_queue(queue_t *q) {
  node_t *nd = new node_t;
  nd->next = {NULL, 0, 0};
  q->tail  = {nd, 0, 0};
  q->head  = {nd, 0, 0};
}

void backoff_scheme() {
  // nanosleep((const struct timespec[]){{0, 500L}}, NULL);
  usleep(1);
}

bool baskets_enqueue(queue_t *q, size_t val, bool test) {
  node_t *nd = new node_t;
  nd->value = val;
  while(true) {
    pointer_t tail = q->tail;
    pointer_t next = tail.ptr->next;
    if (tail.ptr == q->tail.ptr) {
      if (next.ptr == NULL) {
        nd->next = {NULL, 0, tail.tag+2};
        if (CAS(&tail.ptr->next.ptr, next.ptr, nd) &&
          CAS(&tail.ptr->next.deleted, next.deleted, 0) &&
          CAS(&tail.ptr->next.tag, next.tag, tail.tag+1)) {
            CAS(&q->tail.ptr, tail.ptr, nd);
            CAS(&q->tail.deleted, tail.deleted, 0);
            CAS(&q->tail.tag, tail.tag, tail.tag+1);
            if(test) {
              printf("%i ", nd->value);
              if(nd->value%15==0) printf("\n Popped: ");
            }
            return true;
        }
        next = tail.ptr->next;
        while((next.tag == tail.tag+1) && (!next.deleted)) {
          backoff_scheme();
          nd->next = next;
          if (CAS(&tail.ptr->next.ptr, next.ptr, nd) &&
            CAS(&tail.ptr->next.deleted, next.deleted, 0) &&
            CAS(&tail.ptr->next.tag, next.tag, tail.tag+1)){
              return true;
          }
          next = tail.ptr->next;
        }
      } else {
        while ((next.ptr->next.ptr != NULL) && (q->tail.ptr == tail.ptr)) {
          next = next.ptr->next;
        }
        CAS(&q->tail.ptr, tail.ptr, nd);
        CAS(&q->tail.deleted, tail.deleted, 0);
        CAS(&q->tail.tag, tail.tag, tail.tag+1);
      }
    }
  }
  return false;
}

void free_chain(queue_t *q, pointer_t head, pointer_t new_head) {
  if (CAS(&q->head.ptr, head.ptr, new_head.ptr) &&
    CAS(&q->head.deleted, head.deleted, 0) &&
    CAS(&q->head.tag, head.tag, head.tag+1)) {
      while (head.ptr != new_head.ptr) {
        pointer_t next = head.ptr->next;
        // reclaim_node(head.ptr); // not sure what this is from paper
        head = next;
      }
    }
}

int baskets_dequeue(queue_t *q, bool test) {
  while(true) {
    pointer_t head = q->head;
    pointer_t tail = q->tail;
    pointer_t next = head.ptr->next;
    if (head.ptr == q->head.ptr) {
      if (head.ptr == tail.ptr) {
        if (next.ptr == NULL) return -1;
        while ((next.ptr->next.ptr != NULL) && (q->tail.ptr == tail.ptr))
          next = next.ptr->next;
        CAS(&q->tail.ptr, tail.ptr, next.ptr);
        CAS(&q->tail.deleted, tail.deleted, 0);
        CAS(&q->tail.tag, tail.tag, tail.tag+1);
      } else {
        pointer_t iter = head;
        size_t hops = 0;
        while ((next.deleted && iter.ptr != tail.ptr) && (q->head.ptr == head.ptr)) {
          iter = next;
          next = iter.ptr->next;
          hops++;
        }
        if (q->head.ptr != head.ptr) continue;
        else if (iter.ptr == tail.ptr) free_chain(q, head, iter);
        else {
          size_t value = next.ptr->value;
          if (CAS(&iter.ptr->next.ptr, next.ptr, next.ptr) &&
            CAS(&iter.ptr->next.deleted, next.deleted, 1) &&
            CAS(&iter.ptr->next.tag, next.tag, next.tag+1)) {
              if (hops >= MAX_HOPS) free_chain(q, head, iter);
              if(test) printf("%i ", value);
              return value;
          } backoff_scheme();
        }
      }
    }
  }
  return -1;
}

#endif // BASKETS_QUEUE
#endif // __APPLE__