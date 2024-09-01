#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);
long search(int n, int loop);

int main(int argc, char *argv[]) {
  int k = 100;
  int loop = 500;
  for (int n = 1000; n <= 4096000; n += 273000) {
    long max_time = LONG_MIN;
    long min_time = LONG_MAX;
    long total_time = 0;
    for (int i = 0; i < k; i++) {
      long wall = search(n, loop);
      total_time += wall;
      if (wall < min_time) min_time = wall;
      if (wall > max_time) max_time = wall;
    }
    long avg_time = total_time/k;
    printf("%d %0.2f %0.2f %0.2f\n", 
            n, (double)min_time/loop, (double)max_time/loop, (double)avg_time/loop);
  }
}

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
  (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

long search(int n, int loop) {
  int *array = (int*)malloc(n*sizeof(int));
  for (int i = 0; i < n; i++) array[i] = rand()%(n*2);
  
  int *keys = (int*)malloc(loop*sizeof(int));
  for (int i = 0; i < loop; i++) keys[i] = rand()%(n*2);

  int sum = 0;
  clock_gettime(CLOCK_MONOTONIC, &t_start);
  for (int i = 0; i < loop; i++) {
    int key = keys[i];
    for (int j = 0; j < n; j++) {
      if (key == array[j]) {
        sum++;
        break;
      }
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &t_stop);

  if(sum == 0) return 0;

  long wall = nano_seconds(&t_start, &t_stop);
  return wall;
}