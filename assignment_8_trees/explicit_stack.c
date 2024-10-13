#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int value;
  struct node *left, *right;
} node;

typedef struct tree {
  node *root;
} tree;

typedef struct stack_node {
  node *tree_node;
  struct stack_node *next;
} stack_node;

typedef struct stack {
  stack_node *top;
} stack;

tree *construct_tree() {
  tree *tr = (tree *)malloc(sizeof(tree));
  tr->root = NULL;
  return tr;
}

node *construct_node(int value) {
  node *nd = (node *)malloc(sizeof(node));
  nd->value = value;
  nd->left = NULL;
  nd->right = NULL;
  return nd;
}

stack *create_stack() {
  stack *stk = (stack *)malloc(sizeof(stack));
  stk->top = NULL;
  return stk;
}

void push(stack *stk, node *tree_node) {
  stack_node *new_node = (stack_node *)malloc(sizeof(stack_node));
  new_node->tree_node = tree_node;
  new_node->next = stk->top;
  stk->top = new_node;
}

node *pop(stack *stk) {
  if (stk->top == NULL) {
    return NULL;
  }
  stack_node *temp = stk->top;
  node *res = temp->tree_node;
  stk->top = stk->top->next;
  free(temp);
  return res;
}

int is_empty(stack *stk) {
  return stk->top == NULL;
}

void print_in_order(tree *tr) {
    stack *stk = create_stack();
    node *cur = tr->root;

    while (1) {
        if (cur == NULL && is_empty(stk)) {
          break;
        }         
        
        // 不斷移動到左子樹，將節點壓入堆疊
        while (cur != NULL) {
          push(stk, cur);
          cur = cur->left;
        }

        // 彈出最左節點並打印
        cur = pop(stk);
        printf("%d ", cur->value);

        // 移動到右子樹
        cur = cur->right;
    }

    free(stk);
}

// 插入節點到二叉搜索樹
void add(tree *tr, int value) {
  if (tr->root == NULL) {
    tr->root = construct_node(value);
    return;
  }

  node *current = tr->root;
  while (1) {
    if (value == current->value) {
      return;
    } else if (value < current->value) {
      if (current->left == NULL) {
        current->left = construct_node(value);
        return;
      } else {
        current = current->left;
      }
    } else {
      if (current->right == NULL) {
        current->right = construct_node(value);
        return;
      } else {
        current = current->right;
      }
    }
  }
}

void free_node(node *nd) {
  if (nd != NULL) {
    free_node(nd->left);
    free_node(nd->right);
    free(nd);
  }
}

void free_tree(tree *tr) {
  if (tr != NULL) {
    free_node(tr->root);
    free(tr);
  }
}

int main() {
  tree *tr = construct_tree();
  add(tr, 5);
  add(tr, 3);
  add(tr, 7);
  add(tr, 2);
  add(tr, 4);
  add(tr, 6);
  add(tr, 8);

  printf("In-order traversal:\n");
  print_in_order(tr);

  free_tree(tr);
  return 0;
}
