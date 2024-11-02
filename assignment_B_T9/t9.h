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

#define BUFFER 3000
#define ALPHABET_SIZE 30

typedef struct node {
  bool valid;
  struct node *next[30];
} node;

typedef struct trie {
  node *root;
} trie;

typedef struct word_list {
  wchar_t **words;
  int count;
  int capacity;
} word_list;

// Mapping of T9 keys to character ranges
static const int key_start[] = {
  0,    // 1: abc (0,1,2)
  3,    // 2: def (3,4,5)
  6,    // 3: ghi (6,7,8)
  9,    // 4: jkl (9,10,11)
  12,   // 5: mno (12,13,14)
  15,   // 6: pqrs (15,16,17,18)
  19,   // 7: tuv (19,20,21)
  22,   // 8: wxyz (22,23,24,25)
  26    // 9: åäö (26,27,28)
};

static const int key_length[] = {
  3,  // 1: abc
  3,  // 2: def
  3,  // 3: ghi
  3,  // 4: jkl
  3,  // 5: mno
  4,  // 6: pqrs
  3,  // 7: tuv
  4,  // 8: wxyz
  3   // 9: åäö
};

// T9 methods
trie *dict();
node *add(node *nd, wchar_t *rest);
static int code(wchar_t w);
wchar_t reverse_code(int code);
void combine_unicode(wchar_t *str, wchar_t *output);
void test_combine_unicode();
void test_add();

// Print methods
void print_unicode(wchar_t *str);
void print_method1(wchar_t *str);
void print_method2(wchar_t *str);
void print_method3(wchar_t *str);

// Decode methods
void add_word(word_list *list, wchar_t *word);
void collect(node *current, const char *keys, int key_pos, wchar_t *prefix, int prefix_pos, word_list *results);
wchar_t **decode(trie *tr, const char *sequence, int *count);
void free_decoded(wchar_t **words, int count);

#endif // _T9_H_