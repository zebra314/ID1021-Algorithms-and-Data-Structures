#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

#include "tool_func.h"

void test_clock_accuracy();
void single_array_access();
void simple_random_access();
void simple_bench();
void diff_sizes_bench();

int main(int argc, char *argv[]) {    
  // test_clock_accuracy();
  // single_array_access();
  // simple_random_access();
  // simple_bench();
  diff_sizes_bench();
}
    
void test_clock_accuracy() {
  for(int i = 0; i < 10; i++) {
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    printf("%ld ns\n", wall); 
  }
}

void single_array_access() {
  int given[] = {1,2,3,4,5,6,7,8,9,0};
  int sum = 0;
  for(int i = 0; i < 10; i++) {
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    sum += given[i];
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    printf("one operation in %ld ns\n", wall);
  }
}

void simple_random_access() {
  int array[] = {0,1,2,3,4,5,6,7,8,9};
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    sum += array[rand()%10];
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    printf("%ld ns\n", wall);
  }
}

void simple_bench() {
  int n = 100;
  int loop = 1000;
  for (int i = 0; i < 10; i++) {
    long wall = bench(n, loop);
    printf("time : %lld ns\n", wall);
  }
}

void diff_sizes_bench() {
  // int sizes[] = {1000,2000,4000,8000,16000,32000};
  int k = 10;
  int loop = 1000;
  for (int n = 1000; n <= 32000; n *= 2) {
    long min = LONG_MAX;
    for (int i = 0; i < k; i++) {
      long wall = bench(n, loop);
      if (wall < min)
        min = wall;
    }
    printf("%d %0.2f ns\n", n, (double)min/loop);
  }
}