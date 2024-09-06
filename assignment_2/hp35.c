#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct stack {
  int top;
  int size;
  int *array;
} stack;

stack *new_stack(int size) {
  int *array = (int*)malloc(size*sizeof(int));
  stack *stk = (stack*)malloc(sizeof(stack));
  stk->top = 0; // Point to the above the stack.
  stk->size = size;
  stk->array = array; 
  return stk;
}

void push(stack *stk, int val) {
  if (stk->top == stk->size) {
    int size = stk->size * 2;
    int *copy = (int*)malloc(size*sizeof(int));
    stk->size = size;
    for (int i = 0; i < stk->top; i++) {
      copy[i] = stk->array[i];
    }
    free(stk->array);
    stk->array = copy;
  }
  stk->array[stk->top] = val;
  stk->top++;
}

int pop(stack *stk) {
  if(stk->top == 0) {
    printf("Underflow");
    return -1;
  } else {
    stk->top--;
    return stk->array[stk->top];
  }
}

int main() {
  printf("HP-35 pocket calculator\n");
  stack *stk = new_stack(10);
  size_t n = 10;
  char *buffer = (char*)malloc(n*sizeof(char));
  bool run = true;
  while(run) {
    printf(" > ");
    getline(&buffer, &n, stdin);
    if (strcmp(buffer, "\n") == 0) {
      run = false;
    } else if(strcmp(buffer, "+\n") == 0) {
      int a = pop(stk);
      int b = pop(stk);
      push(stk, b+a);
    } else if(strcmp(buffer, "-\n") == 0) {
      int a = pop(stk);
      int b = pop(stk);
      push(stk, b-a);
    } else if(strcmp(buffer, "*\n") == 0) {
      int a = pop(stk);
      int b = pop(stk);
      push(stk, b*a);
    } else if(strcmp(buffer, "/\n") == 0) {
      int a = pop(stk);
      int b = pop(stk);
      push(stk, b/a);
    } else {
      int val = atoi(buffer);
      push(stk, val);
    }
  }
  printf("the result is: %d\n\n", pop(stk));
  printf("I love reversed polish notation, don't you?\n");
}

/*
4 2 3 * 4 + 4 * + 2 -
4 6 4 + 4 * + 2 -
4 10 4 * + 2 -
4 40 + 2 -
44 2 -
42
*/