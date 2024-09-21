#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>


typedef struct {
  int ***array_list; // [[array1, array2, ...xloop], [array1, array2, ...xloop], ...xn]
} TestData;

// Tool functions
struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);
int *get_unsorted_array(int length);
TestData get_test_data(int array_size_list[], int n, int loop);

// Sorting function
void selection_sort(int *array, int length);

int main() {
  srand(time(NULL));  // Seed the random number generator
  int loop = 100;
  int array_size[] = {10, 100, 1000, 10000, 100000};
  int n = sizeof(array_size) / sizeof(array_size[0]);
  TestData test_data = get_test_data(array_size, n, loop);

  for(int i = 0; i < n; i++) {
    int length = array_size[i];

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for(int j = 0; j < loop; j++) {
      int *array = test_data.array_list[i][j];
      selection_sort(array, length);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);

    long elapsed_time = nano_seconds(&t_start, &t_stop);
    printf("%d %ld\n", length, elapsed_time/loop);
  }

  // Free memory
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < loop; j++) {
      free(test_data.array_list[i][j]);
    }
    free(test_data.array_list[i]);
  }
}

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
  (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

TestData get_test_data(int array_size_list[], int n, int loop) {
  TestData test_data;
  test_data.array_list = (int***)malloc(n*sizeof(int**));
  for(int i = 0; i < n; i++) {
    test_data.array_list[i] = (int**)malloc(loop*sizeof(int*));
    for(int j = 0; j < loop; j++) {
      int length = array_size_list[i];
      int *array = get_unsorted_array(length);
      test_data.array_list[i][j] = array;
    }
  }
  return test_data;
}

int *get_unsorted_array(int length) {
  int *array = (int*)malloc(length*sizeof(int));
  for(int i = 0; i < length; i++) {
    array[i] = rand();
  }
  return array;
}

void selection_sort(int *array, int length) {
  for (int i = 0; i < length; i++) {
    int candidate = array[i];
    for (int j = i; j < length ; j++) {
      candidate = array[j] < candidate ? array[j] : candidate; 
    }
    array[i] = candidate;
  }
}