#ifndef TREES_H
#define TREES_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

typedef struct node {
  int value;
  struct node *right;
  struct node *left;
} node;

typedef struct tree {
  node *root;
} tree;

tree *construct_tree();
node *construct_node(int val);
void free_tree(tree *tr);
void free_node(node *nd);
void add(tree *tr, int value);
void add_recursive(node *current, int value);
void print_pre_ord(node *nd);
void print_in_ord(node *nd);
void print_post_ord(node *nd);
void print_tree(tree *tr);

#endif // TREES_H