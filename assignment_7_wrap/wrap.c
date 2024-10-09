#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_SIZE 2

typedef struct queue {
  int first;
  int last;
  int size;
  int count;
  int* queue;
} queue;

queue* create_queue() {
  queue* q = (queue*)malloc(sizeof(queue));
  q->size = DEFAULT_SIZE;
  q->queue = (int*)malloc(q->size * sizeof(int));
  q->count = 0;  
  q->first = 0;
  q->last = 0;
  return q;
}

void enqueue(queue* q, int data) {
  if (q->count == q->size) {  // If the queue is full
    printf("Overflow, increasing the size of the queue\n");
    q->size *= 2;
    int* copy = (int*)malloc(q->size * sizeof(int));

    int index;
    for (int i = 0; i < q->count; i++) {
      index = (q->first + i) % q->size;
      copy[i] = q->queue[index];
    }

    q->first = 0;
    q->last = q->count;
    free(q->queue);
    q->queue = copy;
  }

  q->queue[q->last] = data;
  q->last = (q->last + 1) % q->size;
  q->count++;
}

void dequeue(queue* q) {
  if (q->count == 0) {
    printf("Empty\n");
    return;
  }
  
  int data = q->queue[q->first];
  printf("Dequeued: %d\n", data);
  q->first = (q->first + 1) % q->size;
  q->count--;
}

void display(queue* q) {
  if (q->count == 0) {
    printf("Empty\n");
    return;
  }

  int index;
  for (int i = 0; i < q->count; i++) {
    index = (q->first + i) % q->size;
    printf("%d, ", q->queue[index]);
  }
  printf("\n");
}

int main() {
  queue* q = create_queue();

  display(q);

  enqueue(q, 20);
  enqueue(q, 30);
  enqueue(q, 40);
  enqueue(q, 50);

  display(q);

  enqueue(q, 60);

  display(q);

  dequeue(q);
  dequeue(q);

  printf("After two dequeues\n");

  display(q);

  printf("After one enqueue\n");
  enqueue(q, 60);

  display(q);

  free(q->queue);
  free(q);

  return 0;
}
