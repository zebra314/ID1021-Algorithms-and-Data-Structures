#include "bfs.h"

/* ----------------------------- Tree operations ---------------------------- */

TreeNode* create_node(int value) {
  TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
  node->value = value;
  node->left = node->right = NULL;
  return node;
}

Tree* create_tree() {
  Tree* tree = (Tree*)malloc(sizeof(Tree));
  tree->root = NULL;
  return tree;
}

/* ---------------------------- Queue operations ---------------------------- */

Queue* create_queue() {
  Queue* queue = (Queue*)malloc(sizeof(Queue));
  queue->front = NULL;
  queue->rear = NULL;
  return queue;
}

void enqueue(Queue* queue, TreeNode* tree_node) {
  // Create a new queue node
  QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
  new_node->tree_node = tree_node;
  new_node->next = NULL;
  
  // If the queue is empty
  if (queue->rear == NULL) {
    queue->front = new_node;
    queue->rear = new_node;
  }

  // Add the new node at the end of queue
  queue->rear->next = new_node;
  queue->rear = new_node;
}

TreeNode* dequeue(Queue* queue) {
  if (queue->front == NULL) {
    return NULL;
  }
  
  // Get the first node
  TreeNode* tree_node = queue->front->tree_node;
  
  // Move the second node to the front
  queue->front = queue->front->next;
  if (queue->front == NULL) {
    queue->rear = NULL;
  }
  
  return tree_node;
}

/* --------------------------- Sequence operations -------------------------- */

Sequence* create_sequence(Tree* tree) {
  Sequence* seq = (Sequence*)malloc(sizeof(Sequence));
  seq->queue = create_queue();
  if (tree != NULL) {
    enqueue(seq->queue, tree->root);
  }
  return seq;
}

int next(Sequence* seq) {
  if (seq->queue->front == NULL) {
    return -1; // Indicating end of sequence
  }
  
  TreeNode* current = dequeue(seq->queue);
  int value = current->value;
  
  // Add children to queue for future processing
  if (current->left != NULL) {
    enqueue(seq->queue, current->left);
  }
  if (current->right != NULL) {
    enqueue(seq->queue, current->right);
  }
  
  return value;
}

/* ------------------------------- Free memory ------------------------------ */

void free_sequence(Sequence* seq) {
  while (seq->queue->front != NULL) {
    dequeue(seq->queue);
  }
  free(seq->queue);
  free(seq);
}

void free_tree(TreeNode* node) {
  if (node == NULL) {
    return;
  }
  free_tree(node->left);
  free_tree(node->right);
  free(node);
}
