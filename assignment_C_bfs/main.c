#include "bfs.h"

int main() {
  // Initialize the tree
  Tree* tree = construct_tree();

  // Create sequence
  Sequence* seq = create_sequence(tree);
  
  // Find the value
  int current;
  while(true) {
    current = next(seq);

    if(current == -1) {
      printf("End searching, target not found");
      break;
    } else if (current == 30) {
      printf("Found target 30");
      break;
    }
  }

  // Free memory
  free_tree(tree->root);
  free_sequence(seq);

  return 0;
}