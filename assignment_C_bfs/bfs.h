#ifndef _BFS_H_
#define _BFS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Node structure for the binary tree
typedef struct TreeNode {
  int value;
  struct TreeNode* left;
  struct TreeNode* right;
} TreeNode;

// Node structure for the queue
typedef struct QueueNode {
  TreeNode* tree_node;
  struct QueueNode* next;
} QueueNode;

// Queue structure
typedef struct Queue {
  QueueNode* front;
  QueueNode* rear;
} Queue;

// Sequence structure to maintain BFS state
typedef struct Sequence {
  Queue* queue;
} Sequence;

Queue* create_queue();
void enqueue(Queue* queue, TreeNode* tree_node);
TreeNode* dequeue(Queue* queue);
TreeNode* create_node(int value);
Sequence* create_sequence(TreeNode* root);
int next(Sequence* seq);
void free_sequence(Sequence* seq);

#endif // _BFS_H_
