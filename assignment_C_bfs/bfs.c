#include "bfs.h"

/* ----------------------------- Tree operations ---------------------------- */

TreeNode* create_node(int value) {
  TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  return node;
}

Tree* create_tree() {
  Tree* tree = (Tree*)malloc(sizeof(Tree));
  tree->root = NULL;
  return tree;
}

Tree* construct_tree() {
  Tree* tree = (Tree*)malloc(sizeof(Tree));

  TreeNode* a = create_node(2);
  TreeNode* b = create_node(4);
  TreeNode* c = create_node(6);
  TreeNode* d = create_node(8);
  TreeNode* e = create_node(10);
  TreeNode* f = create_node(12);
  TreeNode* g = create_node(14);
  TreeNode* h = create_node(16);
  TreeNode* i = create_node(18);
  TreeNode* j = create_node(20);
  TreeNode* k = create_node(22);
  TreeNode* l = create_node(24);
  TreeNode* m = create_node(26);
  TreeNode* n = create_node(28);
  TreeNode* o = create_node(30);

  tree->root = a;
  a->left = b;
  a->right = c;
  b->left = d;
  b->right = e;
  c->left = f;
  c->right = g;
  d->left = h;
  d->right = i;
  e->left = j;
  e->right = k;
  f->left = l;
  f->right = m;
  g->left = n;
  g->right = o;

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

  // Queue is empty
  if (queue->front == NULL && queue->rear == NULL) {
    queue->front = new_node;
    queue->rear = new_node;
    return;
  }

  // Add the new node at the end of queue
  queue->rear->next = new_node;
  queue->rear = new_node;
}

TreeNode* dequeue(Queue* queue) {
  // Queue is empty
  if(queue->front == NULL && queue->rear == NULL) {
    printf("Queue is empty\n");
    return NULL;
  }

  // Get the first node
  TreeNode* tree_node = queue->front->tree_node;

  // Queue has only one node
  if (queue->front == queue->rear) {
    queue->front = NULL;
    queue->rear = NULL;
  } else {
    // Move the front pointer to the next node
    queue->front = queue->front->next;
  }

  return tree_node;
}

/* --------------------------- Sequence operations -------------------------- */

Sequence* create_sequence(Tree* tree) {
  Sequence* seq = (Sequence*)malloc(sizeof(Sequence));
  seq->queue = create_queue();
  if (tree->root != NULL) {
    enqueue(seq->queue, tree->root);
  }
  return seq;
}

int next(Sequence* seq) {
  if (seq->queue->front == NULL) {
    return -1;
  }

  // Get the value of the current node
  TreeNode* current = dequeue(seq->queue);
  int value = current->value;
  printf("Visited node: %d\n", value);

  // Add children to queue
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
