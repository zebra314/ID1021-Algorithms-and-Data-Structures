#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
  int length;
  int *array;
} Array;

typedef struct {
  Array **array_list; // [[array1, array2, ...xloop], [array1, array2, ...xloop], ...xn]
} TestData;

// Tool functions
struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);
int *get_unsorted_array(int length);
TestData get_test_data(int array_size_list[], int n, int loop);
bool check_sorted(int *array, int length);

// Sorting functions
Array quick_sort(Array array);

int main() {
  srand(time(NULL));  // Seed the random number generator
  int loop = 500;
  int array_size[] = {10, 100, 1000, 10000, 100000};
  int n = sizeof(array_size) / sizeof(array_size[0]);
  TestData test_data = get_test_data(array_size, n, loop);

  for(int i = 0; i < n; i++) {
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for(int j = 0; j < loop; j++) {
      Array array = test_data.array_list[i][j];
      Array sorted = quick_sort(array);
      free(sorted.array);  // Free the sorted array
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);

    long elapsed_time = nano_seconds(&t_start, &t_stop);
    printf("%d %ld\n", array_size[i], elapsed_time/loop);
  }

  // Free memory
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < loop; j++) {
      free(test_data.array_list[i][j].array);
    }
    free(test_data.array_list[i]);
  }
  free(test_data.array_list);
}

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
         (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

TestData get_test_data(int array_size_list[], int n, int loop) {
  TestData test_data;
  test_data.array_list = (Array**)malloc(n * sizeof(Array*));
  for(int i = 0; i < n; i++) {
    test_data.array_list[i] = (Array*)malloc(loop * sizeof(Array));
    for(int j = 0; j < loop; j++) {
      Array array;
      array.length = array_size_list[i];
      array.array = get_unsorted_array(array.length);
      test_data.array_list[i][j] = array;
    }
  }
  return test_data;
}

int *get_unsorted_array(int length) {
  int *array = (int*)malloc(length * sizeof(int));
  for(int i = 0; i < length; i++) {
    array[i] = rand();
  }
  return array;
}

bool check_sorted(int *array, int length) {
  for (int i = 0; i < length - 1; i++) {
    if (array[i] > array[i + 1]) {
      return false;
    }
  }
  return true;
}

Array quick_sort(Array array) {
  if (array.length <= 1) return array;

  Array left, right;
  left.length = array.length / 2;
  right.length = array.length - left.length;

  left.array = (int*)malloc(left.length * sizeof(int));
  right.array = (int*)malloc(right.length * sizeof(int));

  for (int i = 0; i < left.length; i++) {
    left.array[i] = array.array[i];
  }
  for (int i = 0; i < right.length; i++) {
    right.array[i] = array.array[left.length + i];
  }

  left = merge_sort(left);
  right = merge_sort(right);

  Array merged = merge_array(left, right);

  // Free temporary arrays
  free(left.array);
  free(right.array);

  return merged;
}
