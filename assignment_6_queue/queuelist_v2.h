#ifndef QUEUELIST_V2_H
#define QUEUELIST_V2_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

typedef struct node_v2 {
  int value;
  struct node_v2* next;
} node_v2;

typedef struct queue_v2 {
  node_v2* first;
  node_v2* last;
} queue_v2;

queue_v2* create_queue_v2();
void enque_v2(queue_v2* q, int value);
int dequeue_v2(queue_v2* q);
int empty_v2(queue_v2* q);

#endif // QUEUELIST_V2_H