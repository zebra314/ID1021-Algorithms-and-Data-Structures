#include "queuelist_v2.h"

queue_v2 *create_queue_v2() {
  queue_v2 *q = (queue_v2*)malloc(sizeof(queue_v2));
  q->first = NULL;
  q->last = NULL;
  return q;
}

int empty_v2(queue_v2 *q) {
  return q->first == NULL;
}

void enque_v2(queue_v2* q, int v) {
  // Init the new element
  node_v2 *nd = (node_v2*)malloc(sizeof(node_v2));
  nd->value = v;
  nd->next = NULL;

  // Check if the new one is the first element in the queue_v2
  if (q->last != NULL) {
    q->last->next = nd;
  } else {
    q->first = nd;
  }
  q->last = nd;
}

int dequeue_v2(queue_v2 *q) {
  int res = 0;
  if (q->first != NULL) {
    node_v2* temp = q->first;
    res = q->first->value;
    q->first = q->first->next;
    free(temp);
  }
  return res;
}