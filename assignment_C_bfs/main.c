#include "bfs.h"

int main() {
  // Initialize
  Tree* tree = create_tree();
  Queue* queue = create_queue();
  
  // Create a binary tree

  // Create sequence
  Sequence* seq = create_sequence(tree);
  
  // Free memory
  free_tree(tree->root);
  free_sequence(seq);

  return 0;
}