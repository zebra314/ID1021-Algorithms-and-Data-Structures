#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);
long bench(int n, int loop);
void test_clock_accuracy();
void single_array_access();
void simple_random_access();
void simple_bench();
void diff_sizes_bench();

long test(){
  clock_gettime(CLOCK_MONOTONIC, &t_start);
  clock_gettime(CLOCK_MONOTONIC, &t_stop);
  long wall = nano_seconds(&t_start, &t_stop);
  return wall;
}

int main(int argc, char *argv[]) {    
  int k = 10;
  int loop = 25;
  for (int n = 256000; n < 262144000; n *= 2) {
    long min_time = LONG_MAX;
    long max_time = 0;        
    long total_time = 0;
    for (int i = 0; i < k; i++) {
      long wall = bench(n, loop);
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
    
void test_clock_accuracy() {
  for(int i = 1; i < 1000; i++) {
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    printf("%d %ld\n", i, wall); 
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
  int k = 10;
  int loop = 1000;
  for (int n = 1000; n <= 32000; n *= 2) {

    // Time measurement
    long max_time = LONG_MIN;
    long min_time = LONG_MAX;
    long total_time = 0;

    for (int i = 0; i < k; i++) {
      long wall = bench(n, loop);
      total_time += wall;
      if (wall < min_time) min_time = wall;
      if (wall > max_time) max_time = wall;
    }
 
    long avg_time = total_time/k;
    printf("%d, min: %0.2f, max: %0.2f, avg: %0.2f\n", 
            n, (double)min_time/loop, (double)max_time/loop, (double)avg_time/loop);
  }
}
