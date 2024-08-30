#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

#include "tool_func.h"

long duplicates(int n, int loop);

int main(int argc, char *argv[]) {
  // int sizes[] = {1000,2000,4000,8000,16000,32000};
  int k = 10;
  int loop = 1000;
  for (int n = 1000; n <= 32000; n *= 2) {
    long min = LONG_MAX;
    for (int i = 0; i < k; i++) {
      long wall = duplicates(n, loop);
      if (wall < min)
        min = wall;
    }
    printf("%d %0.2f ns\n", n, (double)min/loop);
  }
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