#ifndef _T9_H_
#define _T9_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
#include <iconv.h>

#define BUFFER 1024

typedef struct node {
  bool valid;
  struct node *next[27];
} node;

typedef struct trie {
  node *root;
} trie;

trie *dict();
node *add(node *nd, wchar_t *rest);
static int code(wchar_t w);
void combine_unicode(wchar_t *str, wchar_t *result);
void test_combine_unicode();

// Print methods
void print_unicode(wchar_t *str);
void print_method1(wchar_t *str);
void print_method2(wchar_t *str);
void print_method3(wchar_t *str);

#endif // _T9_H_