#include "t9.h"

int main() {
  trie *kelly = dict();
  node *current = kelly->root;

  char* sequence = "277";  // "ett"
  int count;
  wchar_t** words = decode(kelly, sequence, &count);

  // Print results
  for (int i = 0; i < count; i++) {
    print_method3(words[i]);
  }
  printf("Words: %d\n", count);
  
  // Free memory
  free_decoded(words, count);

  return 0;
}