#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
  (t_stop->tv_sec - t_start->tv_sec)*1000000000;
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
  int *indx = (int*)malloc(n*sizeof(int));
  for (int i = 0; i < loop; i++) indx[i] = rand()%n;

  int sum = 0;
  clock_gettime(CLOCK_MONOTONIC, &t_start);
  for (int i = 0; i < loop; i++) sum += array[indx[i]];
  clock_gettime(CLOCK_MONOTONIC, &t_stop);
  long wall = nano_seconds(&t_start, &t_stop);
  return wall;
}

int main() {    
  int n = 100;
  int loop = 104;
  for (int i = 0; i < 10; i++) {
    long wall = bench(n, loop);
    printf("time : %lld ns\n", wall);
  }
}