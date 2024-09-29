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
  stack *stk = new_stack(4);
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
}