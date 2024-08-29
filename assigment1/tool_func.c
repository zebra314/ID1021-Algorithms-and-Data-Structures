#include "tool_func.h"

struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
  (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

/*
 * Benchmark the time to access n elements in an array
 * n : number of elements in the array
 * loop : number of times to access the array (with random index each time)
 */
long bench(int n, int loop) {

  // Init key array
  int *array = (int*)malloc(n*sizeof(int));
  for (int i = 0; i < n; i++) array[i] = i;

  // Init random index array
  int *indx = (int*)malloc(loop*sizeof(int));
  for (int i = 0; i < loop; i++) indx[i] = rand()%n;

  int sum = 0;
  clock_gettime(CLOCK_MONOTONIC, &t_start);
  for (int i = 0; i < loop; i++) sum += array[indx[i]];
  clock_gettime(CLOCK_MONOTONIC, &t_stop);

  if (sum == 0)
    return 0;

  long wall = nano_seconds(&t_start, &t_stop);
  return wall;
}