#include "bfs.h"

void enqueue(Queue* queue, TreeNode* tree_node) {
  QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
  new_node->tree_node = tree_node;
  new_node->next = NULL;
  
  if (queue->rear == NULL) {
    queue->front = queue->rear = new_node;
    return;
  }
  
  queue->rear->next = new_node;
  queue->rear = new_node;
}

TreeNode* dequeue(Queue* queue) {
  if (queue->front == NULL) {
    return NULL;
  }
  
  QueueNode* temp = queue->front;
  TreeNode* tree_node = temp->tree_node;
  
  queue->front = queue->front->next;
  if (queue->front == NULL) {
    queue->rear = NULL;
  }
  
  free(temp);
  return tree_node;
}

// Tree operations
TreeNode* create_node(int value) {
  TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
  node->value = value;
  node->left = node->right = NULL;
  return node;
}

// Sequence operations
Sequence* create_sequence(TreeNode* root) {
  Sequence* seq = (Sequence*)malloc(sizeof(Sequence));
  seq->queue = create_queue();
  if (root != NULL) {
    enqueue(seq->queue, root);
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

// Helper function to clean up
void free_sequence(Sequence* seq) {
  while (seq->queue->front != NULL) {
    dequeue(seq->queue);
  }
  free(seq->queue);
  free(seq);
}