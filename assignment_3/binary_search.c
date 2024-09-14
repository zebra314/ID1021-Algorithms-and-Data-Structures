#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>


typedef struct {
  int ***array_list; // [[array1, array2, ...xloop], [array1, array2, ...xloop], ...xn]
  int **key_list;
} TestData;

struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);
int *get_sorted_array(int n);
int get_random_exisited_key(int *array, int size);
bool binary_search(int array[], int length, int key);
TestData get_test_data(int loop, int array_size_list[], int n);

int main() {
  srand(time(NULL));  // Seed the random number generator
  int loop = 500;
  int array_size[] = {10, 100, 1000, 10000, 100000, 1000000};
  int n = sizeof(array_size) / sizeof(array_size[0]);

  TestData test_data = get_test_data(loop, array_size, n);

  for(int i = 0; i < sizeof(array_size) / sizeof(array_size[0]); i++) {
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for(int j = 0; j < loop; j++) {
      binary_search(test_data.array_list[i][j], array_size[i], test_data.key_list[i][j]);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    
    long total_time = nano_seconds(&t_start, &t_stop);
    printf("%d %0.2ld\n", array_size[i], total_time/loop);
  }

  for (int i = 0; i < sizeof(array_size) / sizeof(array_size[0]); i++) {
    free(test_data.key_list[i]);
    free(test_data.array_list[i]);
  }
  free(test_data.key_list);
  free(test_data.array_list);
}

TestData get_test_data(int loop, int array_size_list[], int n) {
  TestData test_data;
  test_data.array_list = (int***)malloc(n*sizeof(int**));
  test_data.key_list = (int**)malloc(n*sizeof(int*));
  for(int i = 0; i < n; i++) {
    test_data.array_list[i] = (int**)malloc(loop*sizeof(int*));
    test_data.key_list[i] = (int*)malloc(loop*sizeof(int));
    for(int j = 0; j < loop; j++) {
      int length = array_size_list[i];
      int *array = get_sorted_array(length);
      int key = get_random_exisited_key(array, length);
      test_data.array_list[i][j] = array;
      test_data.key_list[i][j] = key;
    }
  }
  printf("Test data generated\n");
  return test_data;
}

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
  (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

int *get_sorted_array(int n) {
  int *array = (int*)malloc(n*sizeof(int));
  int nxt = 0;
  for (int i = 0; i < n ; i++) {
    nxt += rand()%(n*2);
    array[i] = nxt;
  }
  return array;
}

int get_random_exisited_key(int *array, int size) {
  int index = rand() % size;
  return array[index];
}

bool binary_search(int array[], int length, int key) {
  int first = 0;
  int last = length-1;
  while (true) {
    // jump to the middle
    int index = (first + last) / 2;
    // printf("%d %d %d %d\n",array[first], array[index], array[last], key);
    if (array[index] == key) {
      return true;
    }

    if (array[index] < key && index < last) {
      // what is the first possible page?
      first = index + 1;
      continue;
    }

    if (array[index] > key && index > first) {
      // what is the last possible page?
      last = index - 1;
      continue;
    }

    // Why do we land here? What should we do?
    return false;
  }
}