#include "queuelist.h"

queue *create_queue() {
  queue *q = (queue*)malloc(sizeof(queue));
  q->first = NULL;
  return q;
}

int empty(queue *q) {
  return q->first == NULL;
}

void enque(queue* q, int v) {
  // Init the new element
  node *nd = (node*)malloc(sizeof(node));
  nd->value = v;
  nd->next = NULL;

  // Find the last element in the queue
  node *prv = NULL;
  node *nxt = q->first;
  while (nxt != NULL) {
    prv = nxt;
    nxt = nxt->next;
  }

  // Check if the new one is the first element in the queue
  if (prv != NULL) {
    prv->next = nd;
  } else {
    q->first = nd;
  }
}

int dequeue(queue *q) {
  int res = 0;
  if (q->first != NULL) {
    node* temp = q->first;  // Save the current first node
    res = temp->value;            // Get the value from the node
    q->first = q->first->next;    // Move the first pointer to the next node
    free(temp);                   // Free the old first node
  }
  return res;
}