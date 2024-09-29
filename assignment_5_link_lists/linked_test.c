#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// Link list data structure
typedef struct cell {
  int value;
  struct cell *tail;
} cell;

typedef struct linked {
  cell *first;
} linked;

// Linked list functions
linked *linked_create();
linked *linked_init(int n);
void linked_free(linked *lnk);
void linked_add(linked *lnk, int item);
void linked_append(linked *a, linked *b);
int linked_length(linked *lnk);
bool linked_find(linked *lnk, int item);
void linked_remove(linked *lnk, int item);

// Tool functions
struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);

// Test functions
void benchmark_1();
void benchmark_2();
void array_append(int **a, int *size_a, int *b, int size_b);
void compare_to_array();

// Stack data structure
typedef struct stack {
  linked *list;
} stack;

// Stack functions
stack *new_stack();
void push(stack *stk, int val);
int pop(stack *stk);
void implement_stack();
void free_stack(stack *stk);

int main() {
  // benchmark_1();
  // benchmark_2();
  // compare_to_array();
  implement_stack();
  return 0;
}

linked *linked_create() {
  linked *new = (linked*)malloc(sizeof(linked));
  new->first = NULL;
  return new;
}

linked *linked_init(int n) {
  linked *a = linked_create();
  for (int i = 0; i < n; i++) {
    linked_add(a, i);
  }
  return a;
}

void linked_free(linked *lnk) {
  cell *curr = lnk->first;
  while (curr != NULL) {
    cell *tmp = curr->tail;
    free(curr);
    curr = tmp;
  }
  free(lnk);
}

// Add the item as the first cell in the list.
void linked_add(linked *lnk, int item) {
  cell *new = (cell*)malloc(sizeof(cell));
  new->value = item;
  new->tail = lnk->first;
  lnk->first = new;
}

// Add linked list b to the end of linked-list a.
void linked_append(linked *a, linked *b) {
  cell *curr = a->first;
  cell *prv = NULL;

  // Find the last cell in the first list(a)
  while(curr != NULL) {
    prv = curr;
    curr = curr->tail;
  }

  // Set the tail of the last cell in the first list(a) 
  // to the first cell in the second list(b)
  if (prv != NULL) {
    prv->tail = b->first;
  } else {
    a->first = b->first;
  }
}

// Return the length of the list.
int linked_length(linked *lnk) {
  int len = 0;
  cell *curr = lnk->first;
  while (curr != NULL) {
    len++;
    curr = curr->tail;
  }
  return len;
}

// Return true or false depending on if the item can be found in the list.
bool linked_find(linked *lnk, int item) {
  cell *curr = lnk->first;
  while (curr != NULL) {
    if (curr->value == item) {
      return true;
    }
    curr = curr->tail;
  }
  return false;
}

// Remove the item if it exists in the list.
void linked_remove(linked *lnk, int item) {
  cell *curr = lnk->first;
  cell *prv = NULL;
  while (curr != NULL) {
    if (curr->value != item) {
      prv = curr;
      curr = curr->tail;
    } else {
      if(prv != NULL) {
        // Link the previous cell to the next cell.
        prv->tail = curr->tail;
      } else {
        // If the first cell is the one to be removed,
        lnk->first = curr->tail;
      }
      cell *tmp = curr->tail;
      free(curr);
      curr = tmp;
    }
  }
}

// Return the time in nano seconds.
long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
         (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

// Vary the size of the first linked list (a) and 
// append it to a fixed size linked list (b).
void benchmark_1() {
  const int FIXED_SIZE = 1000;
  const int MAX_SIZE = 10000;
  const int STEP = 500;
  const int LOOPS = 10;

  // printf("Benchmark 1: Varying size of list a, fixed size of list b\n");
  // printf("Size of a,Time (ns)\n");

  for (int size_a = STEP; size_a <= MAX_SIZE; size_a += STEP) {
    
    linked **linked_as = (linked **)malloc(LOOPS * sizeof(linked *));
    linked **linked_bs = (linked **)malloc(LOOPS * sizeof(linked *));
    
    for (int i = 0; i < LOOPS; i++) {
      linked_as[i] = linked_init(size_a);
    }

    for (int i = 0; i < LOOPS; i++) {
      linked_bs[i] = linked_init(FIXED_SIZE);
    }

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < LOOPS; i++) {
      linked_append(linked_as[i], linked_bs[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);

    long time_taken = nano_seconds(&t_start, &t_stop) / LOOPS;
    printf("%d %ld\n", size_a, time_taken);

    for (int i = 0; i < LOOPS; i++) {
      linked_free(linked_as[i]);
    }
    free(linked_as);
    free(linked_bs);
  }
}


// The length of a fixed and increase the length of b
void benchmark_2() {
  const int FIXED_SIZE = 1000;
  const int MAX_SIZE = 10000;
  const int STEP = 500;
  const int LOOPS = 10;

  // printf("\nBenchmark 2: Fixed size of list a, varying size of list b\n");
  // printf("Size of b,Time (ns)\n");

  for (int size_b = STEP; size_b <= MAX_SIZE; size_b += STEP) {
    linked **linked_as = (linked **)malloc(LOOPS * sizeof(linked *));
    linked **linked_bs = (linked **)malloc(LOOPS * sizeof(linked *));
    
    for (int i = 0; i < LOOPS; i++) {
      linked_as[i] = linked_init(FIXED_SIZE);
    }

    for (int i = 0; i < LOOPS; i++) {
      linked_bs[i] = linked_init(size_b);
    }

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < LOOPS; i++) {
      linked_append(linked_as[i], linked_bs[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);

    long time_taken = nano_seconds(&t_start, &t_stop) / LOOPS;
    printf("%d %ld\n", size_b, time_taken);

    for (int i = 0; i < LOOPS; i++) {
      linked_free(linked_as[i]);
    }
    free(linked_as);
    free(linked_bs);
  }
}

void array_append(int **a, int *size_a, int *b, int size_b) {
  int new_size = *size_a + size_b;
  int *new_array = (int*)malloc(new_size * sizeof(int));
  
  // Copy elements from array a
  for (int i = 0; i < *size_a; i++) {
    new_array[i] = (*a)[i];
  }
  
  // Copy elements from array b
  for (int i = 0; i < size_b; i++) {
    new_array[*size_a + i] = b[i];
  }
}

void compare_to_array() {
  // Benchmark array append
  const int FIXED_SIZE = 1000;
  const int MAX_SIZE = 10000;
  const int STEP = 500;
  const int LOOPS = 10;

  // printf("\nBenchmark 3: Array append\n");
  // printf("Size of b,Time (ns)\n");

  for (int size_a = STEP; size_a <= MAX_SIZE; size_a += STEP) {
    int **array_as = (int **)malloc(LOOPS * sizeof(int *));
    int **array_bs = (int **)malloc(LOOPS * sizeof(int *));

    for (int i = 0; i < LOOPS; i++) {
      array_as[i] = (int*)malloc(size_a * sizeof(int));
    }

    for (int i = 0; i < LOOPS; i++) {
      array_bs[i] = (int*)malloc(FIXED_SIZE * sizeof(int));
    }

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < LOOPS; i++) {
      array_append(&array_as[i], &size_a, array_bs[i], FIXED_SIZE);
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);

    long time_taken = nano_seconds(&t_start, &t_stop) / LOOPS;
    printf("%d %ld\n", size_a, time_taken);

    for (int i = 0; i < LOOPS; i++) {
      free(array_as[i]);
    }
    free(array_as);
    free(array_bs);
  }

}

void implement_stack() {
  stack *stk = new_stack();
  push(stk, 32);
  push(stk, 33);
  push(stk, 34);

  push(stk, 1);
  push(stk, 2);
  
  printf("pop : %d\n", pop(stk));
  printf("pop : %d\n", pop(stk));
  printf("pop : %d\n", pop(stk));

  push(stk, 3);

  printf("pop : %d\n", pop(stk));
  printf("pop : %d\n", pop(stk));
  printf("pop : %d\n", pop(stk));

  free_stack(stk);
}

stack *new_stack() {
  stack *stk = (stack*)malloc(sizeof(stack));
  stk->list = linked_create();
  return stk;
}

void push(stack *stk, int val) {
  linked_add(stk->list, val);
}

int pop(stack *stk) {
  if (stk->list->first == NULL) {
    printf("Underflow\n");
    return -1;
  } else {
    int val = stk->list->first->value;
    cell *temp = stk->list->first;
    stk->list->first = stk->list->first->tail;
    free(temp);
    return val;
  }
}

void free_stack(stack *stk) {
  while (stk->list->first != NULL) {
    cell *temp = stk->list->first;
    stk->list->first = stk->list->first->tail;
    free(temp);
  }
  free(stk->list);
  free(stk);
}