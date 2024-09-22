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
Array merge_sort(Array array);
Array merge_array(Array left, Array right);

// Optimized
Array merge_sort_v2(Array array);
void merge_array_v2(Array* array, int* aux, int left, int right);

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
      // Array sorted = merge_sort(array);
      Array sorted = merge_sort_v2(array);
      // free(sorted.array);  // Free the sorted array
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

Array merge_sort(Array array) {
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

Array merge_array(Array left, Array right) {
  Array merged;
  merged.length = left.length + right.length;
  merged.array = (int*)malloc(merged.length * sizeof(int));

  int i = 0, j = 0, k = 0;
  while (i < left.length && j < right.length) {
    if (left.array[i] < right.array[j]) {
      merged.array[k++] = left.array[i++];
    } else {
      merged.array[k++] = right.array[j++];
    }
  }

  // After the main comparison loop,
  // there may be some elements left in either left or right array.
  while (i < left.length) {
    merged.array[k++] = left.array[i++];
  }

  while (j < right.length) {
    merged.array[k++] = right.array[j++];
  }

  return merged;
}

Array merge_sort_v2(Array array) {
  int* aux = (int*)malloc(array.length * sizeof(int));
  merge_array_v2(&array, aux, 0, array.length - 1);
  free(aux);
  return array;
}

void merge_array_v2(Array* array, int* aux, int left, int right) {
  if (right <= left) return;

  int mid = left + (right - left) / 2;
  merge_array_v2(array, aux, left, mid);
  merge_array_v2(array, aux, mid + 1, right);

  for (int k = left; k <= right; k++) {
    aux[k] = array->array[k];
  }

  int i = left, j = mid + 1;
  for (int k = left; k <= right; k++) {
    if (i > mid) array->array[k] = aux[j++];
    else if (j > right) array->array[k] = aux[i++];
    else if (aux[j] < aux[i]) array->array[k] = aux[j++];
    else array->array[k] = aux[i++];
  }
}
