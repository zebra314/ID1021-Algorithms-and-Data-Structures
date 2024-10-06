#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queuelist.h"
#include "queuelist_v2.h"

#define NUM_SIZES 9
#define OPERATIONS_PER_SIZE 100000

// Tool functions
struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);

void benchmark_queue();
void benchmark_queue_v2();

int main() {
  // benchmark_queue();
  benchmark_queue_v2();
  return 0;
}

// Return the time in nano seconds.
long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
         (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

void benchmark_queue() {
  int sizes[NUM_SIZES] = {10, 50, 100, 500, 1000, 5000, 10000 , 50000, 100000, 500000, 1000000, 2000000, 3000000, 4000000, 5000000};
  for (int i = 0; i < NUM_SIZES; i++) {
    int size = sizes[i];
    queue* q = create_queue();

    // Benchmark enqueue
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int j = 1; j <= size; j++) {
      enque(q, j);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long enqueue_time = nano_seconds(&t_start, &t_stop) / size;

    // Benchmark dequeue
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    int tmp = 0;
    for (int j = 1; j <= size; j++) {
      tmp = dequeue(q);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long dequeue_time = nano_seconds(&t_start, &t_stop) / size;

    printf("%d %ld %ld\n", size, enqueue_time, dequeue_time);

    // Clean up
    while (!empty(q)) {
      dequeue(q);
    }
    free(q);
  }
  return;
}

void benchmark_queue_v2() {
  int sizes[NUM_SIZES] = {10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 2000000, 3000000, 4000000, 5000000};
  for (int i = 0; i < NUM_SIZES; i++) {
    int size = sizes[i];
    queue_v2* q = create_queue_v2();

    // Benchmark enqueue
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int j = 0; j < size; j++) {
      enque_v2(q, j);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long enqueue_time = nano_seconds(&t_start, &t_stop) / size;

    // Benchmark dequeue
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int j = 0; j < size; j++) {
      dequeue_v2(q);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long dequeue_time = nano_seconds(&t_start, &t_stop) / size;

    printf("%d %ld %ld\n", size, enqueue_time, dequeue_time);

    // Clean up
    while (!empty_v2(q)) {
      dequeue_v2(q);
    }
    free(q);
  }
  printf("\n");
}