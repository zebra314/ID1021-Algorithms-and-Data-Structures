#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);
long duplicates(int n, int loop);

int main(int argc, char *argv[]) {
  int k = 10;
  int loop = 1000;
  for (int n = 1000; n <= 32000; n *= 2) {

    // Time measurement
    long max_time = LONG_MIN;
    long min_time = LONG_MAX;
    long total_time = 0;

    for (int i = 0; i < k; i++) {
      long wall = duplicates(n, loop);
      total_time += wall;
      if (wall < min_time) min_time = wall;
      if (wall > max_time) max_time = wall;
    }

    long avg_time = total_time/k;
    printf("%d, min: %0.2f, max: %0.2f, avg: %0.2f\n", 
            n, (double)min_time/loop, (double)max_time/loop, (double)avg_time/loop);
  }
}

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
  (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

long duplicates(int n, int loop) {
  int *array_a = (int*)malloc(n*sizeof(int));
  for (int i = 0; i < n; i++) array_a[i] = rand()%(n*2);

  int *array_b = (int*)malloc(n*sizeof(int));
  for (int i = 0; i < loop; i++) array_b[i] = rand()%(n*2);

  int sum = 0;
  clock_gettime(CLOCK_MONOTONIC, &t_start);
  for (int i = 0; i < loop; i++) {
    int key = array_a[i];
    for (int j = 0; j < n; j++) {
      if (key == array_b[j]) {
        sum++;
        break;
      }
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &t_stop);

  long wall = nano_seconds(&t_start, &t_stop);
  return wall;
}