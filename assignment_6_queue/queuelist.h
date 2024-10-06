#ifndef QUEUELIST_H
#define QUEUELIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

typedef struct node {
  int value;
  struct node *next;
} node;

typedef struct queue {
  node *first;
} queue;

queue *create_queue();
int empty(queue *q);
void enque(queue* q, int v);
int dequeue(queue *q);

#endif // QUEUELIST_H