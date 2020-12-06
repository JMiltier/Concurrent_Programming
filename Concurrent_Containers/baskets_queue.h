#ifdef __APPLE__

#ifndef BASKETS_QUEUE
#define BASKETS_QUEUE

#include <iostream>
#include <atomic>

#define MAX_HOPS 3
#define CAS __sync_val_compare_and_swap

// from backets queue reference (p. 405)
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

bool baskets_enqueue(queue_t *q, size_t val) {
  node_t *nd = new node_t;
  nd->value = val;
  while(true) {
    printf("YOO!1\n");
    pointer_t tail = q->tail;
    pointer_t next = tail.ptr->next;
    pointer_t nnext = {nd, 0, tail.tag+1};
    if (tail.ptr == q->tail.ptr) {
      if (next.ptr == NULL) {
        nd->next = {NULL, 0, tail.tag+2};
        if (CAS(&tail.ptr->next.ptr, next.ptr, nd) &&
          CAS(&tail.ptr->next.deleted, next.deleted, 0) &&
          CAS(&tail.ptr->next.tag, next.tag, tail.tag+1)) {
            CAS(&q->tail.ptr, tail.ptr, nd);
            CAS(&q->tail.deleted, tail.deleted, 0);
            CAS(&q->tail.tag, tail.tag, tail.tag+1);
            return true;
        }
        next = tail.ptr->next;
        while((next.tag == tail.tag+1) && (!next.deleted)) {
          printf("YOO!3\n");
          // backoff_scheme() not sure what this is for?
          nd->next = next;
          if (CAS(&tail.ptr->next.ptr, next.ptr, nd) &&
            CAS(&tail.ptr->next.deleted, next.deleted, 0) &&
            CAS(&tail.ptr->next.tag, next.tag, tail.tag+1))
              return true;
          next = tail.ptr->next;
        }
      } else {
        while ((next.ptr->next.ptr != NULL) && (q->tail.ptr == tail.ptr)) {
          printf("YOO!4\n");
          next = next.ptr->next;
          return false;
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

int baskets_dequeue(queue_t *q) {
  while(true) {
    pointer_t head = q->head;
    pointer_t tail = q->tail;
    pointer_t next = head.ptr->next;
    if (head.ptr == q->head.ptr) {
      if (head.ptr == tail.ptr) {
        if (next.ptr == NULL) return 999;
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
              return value;
          }
        }
      }
    }
  }
}

#endif // BASKETS_QUEUE
#endif // __APPLE__