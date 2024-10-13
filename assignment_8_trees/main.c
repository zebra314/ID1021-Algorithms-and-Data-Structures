#include "trees.h"

int main() {
  tree *tr = construct_tree();
  add(tr, 5);
  add(tr, 3);
  add(tr, 7);
  add(tr, 2);
  add(tr, 4);
  add(tr, 6);
  add(tr, 8);

  print_tree(tr);
  free_tree(tr);
  return 0;
}