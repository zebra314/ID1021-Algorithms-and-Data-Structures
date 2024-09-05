#include <stdlib.h>
#include <stdio.h>

typedef struct stack {
  int top;
  int size;
  int *array;
} stack;

stack *new_stack(int size) {
  int *array = (int*)malloc(size*sizeof(int));
  stack *stk = (stack*)malloc(sizeof(stack));
  stk->top = -1;
  stk->size = size;
  stk->array = array;
  return stk;
}

void push(stack *stk, int val) {
  if(stk->top < (stk->size - 1)) {
    stk->top++;
    stk->array[stk->top] = val;
  } else {
    printf("Overflow");
  }
  return;
}

int pop(stack *stk) {
  if(stk->top == -1) {
    printf("Underflow");
    return -1;
  } else {
    int pop_value = stk->array[stk->top];
    stk->top--;
    return pop_value;
  }
}

int main() {
  stack *stk = new_stack(4);
  push(stk, 32);
  push(stk, 33);
  push(stk, 34);
  printf("pop : %d\n", pop(stk));
  printf("pop : %d\n", pop(stk));
  printf("pop : %d\n", pop(stk));
}