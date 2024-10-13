#include "trees.h"

tree *construct_tree() { 
  tree *tr = (tree*)malloc(sizeof(tree));
  tr->root = NULL;
  return tr;
}

void free_tree(tree *tr) {
  if (tr != NULL) {
    free_node(tr->root);
    free(tr);
  }
}

node *construct_node(int val) {
  node *nd = (node*)malloc(sizeof(node));
  nd->value =val;
  nd->left = NULL;
  nd->right = NULL;
  return nd;
}

void free_node(node *nd) {
  if (nd != NULL) {
    free_node(nd->left);
    free_node(nd->right);
    free(nd);
  }
}

// Not recursive
void add(tree *tr, int value) {
  // The tree is empty
  if(tr->root == NULL) {
    tr->root = construct_node(value);
    return;
  }

  node *current = tr->root;
  while (true) {

    // The value is already in the tree
    if(current->value == value) {
      return;
    }

    // The value is less than the current code
    // Go to the left
    else if (value < current->value) {
      if (current->left == NULL) {
        current->left = construct_node(value);
        return;
      } else {
        current = current->left;
      }
    } 
    
    // The value is greater than the current code
    // Go to the right
    else {
      if (current->right == NULL) {
        current->right = construct_node(value);
        return;
      } else {
        current = current->right;
      }
    }
  }
}


bool lookup(tree *tr, int value) {
  node *current = tr->root;
  while (current != NULL) {

    // Found the value
    if (current->value == value) {
      return true;
    } 
    
    // The value is less than the current code
    // Go to the left
    else if (value < current->value) {
      current = current->left;
    } 
    
    // The value is greater than the current code
    // Go to the right
    else {
      current = current->right;
    }
  }
  return false;
}

void add_recursive(node *current, int value) {
  // Reach the root
  if (current == NULL) {
    current = construct_node(value);
  } 
  
  // The value is already in the tree
  // Do nothing
  else if (current->value == value) {
    return;
  }

  // The value is less than the current code
  // Go to the left
  else if (value < current->value) {
    add_recursive(current->left, value);

  // The value is greater than the current code
  // Go to the right
  } else if (value > current->value) {
    add_recursive(current->right, value);
  }
}

/* -------------------------------------------------------------------------- */
/*                                    Print                                   */
/* -------------------------------------------------------------------------- */

void print_pre_ord(node *nd) {
  if (nd != NULL) {
    printf("%d ", nd->value);
    print_pre_ord(nd->left);
    print_pre_ord(nd->right);
  }
}

void print_in_ord(node *nd) {
  if (nd != NULL) {
    print_in_ord(nd->left);
    printf("%d ", nd->value);
    print_in_ord(nd->right);
  }
}

void print_post_ord(node *nd) {
  if (nd != NULL) {
    print_post_ord(nd->left);
    print_post_ord(nd->right);
    printf("%d ", nd->value);
  }
}

void print_tree(tree *tr) {
  if (tr->root != NULL) {
    print_in_ord(tr->root);
  }
}