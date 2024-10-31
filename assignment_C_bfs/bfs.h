#ifndef _BFS_H_
#define _BFS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct TreeNode {
  int value;
  struct TreeNode* left;
  struct TreeNode* right;
} TreeNode;

typedef struct Tree {
  TreeNode* root;
} Tree;

// Queue structure to implement BFS traversal
typedef struct QueueNode {
  TreeNode* tree_node;
  struct QueueNode* next;
} QueueNode;

typedef struct Queue {
  QueueNode* front;
  QueueNode* rear;
} Queue;

// Sequence structure to maintain BFS state
typedef struct Sequence {
  Queue* queue;
} Sequence;

// Tree operations
TreeNode* create_node(int value);
Tree* create_tree();
Tree* construct_tree();

// Queue operations
Queue* create_queue();
void enqueue(Queue* queue, TreeNode* tree_node);
TreeNode* dequeue(Queue* queue);

// Sequence operations
Sequence* create_sequence(Tree* tree);
int next(Sequence* seq);

// Free memory
void free_tree(TreeNode* node);
void free_sequence(Sequence* seq);

#endif // _BFS_H_
