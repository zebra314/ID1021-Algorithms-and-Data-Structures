#include "t9.h"

trie *dict() {
  setlocale(LC_ALL,"en_US.UTF-8");

  // Open the dictionary in read mode
  FILE *fptr = fopen("./kelly.txt", "r");
  if (fptr == NULL)
    return NULL;
  
  // Create a new trie
  trie *kelly = (trie*)malloc(sizeof(trie));
  kelly->root = NULL;

  // Buffer for reading lines
  char buf[BUFFER * 4];
  wchar_t wbuf[BUFFER];
  wchar_t result[BUFFER];

  while (fgets(buf, sizeof(buf), fptr) != NULL) {
    // Remove newline character
    buf[strcspn(buf, "\n")] = 0;

    // Convert to wide character
    mbstowcs(wbuf, buf, sizeof(buf));
    combine_unicode(wbuf, result);

    // Add word to trie
    kelly->root = add(kelly->root, result);
  }
  fclose(fptr);

  return kelly;
}

node *add(node *nd, wchar_t *rest) {
  if (nd == NULL) {
    nd = (node*)malloc(sizeof(node));
    nd->valid = false;  // Initialize as false
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      nd->next[i] = NULL;  // Initialize each pointer to NULL
    }
  }

  // Check if we've reached the end of the word
  if (*rest == L'\0') {
    nd->valid = true;
    return nd;
  }

  int c = code((int)*rest);
  // printf("%lc, %d\n", *rest, c);

  if (c == -1) {
    printf("Character: %lc (0x%x)\n", *rest, (unsigned int)*rest);
    return nd;
  }

  nd->next[c] = add(nd->next[c], rest + 1);
  return nd;
}

static int code(wchar_t w) {
  if (w >= L'a' && w <= L'z') return w - L'a';
  if (w >= L'A' && w <= L'Z') return w - L'A';
  
  switch (w) {
    case L'å': case L'Å': return 26;
    case L'ä': case L'Ä': return 27;
    case L'ö': case L'Ö': return 28;
    default:
      return -1;
  }
}

wchar_t reverse_code(int code) {
  if (code >= 0 && code <= 25) return L'a' + code;
  
  switch (code) {
    case 26: return L'å';
    case 27: return L'ä';
    case 28: return L'ö';
    default:
      return L'?'; // Return a placeholder for invalid codes
  }
}

void combine_unicode(wchar_t *input, wchar_t *output) {
  while (*input) {
    // Check if current character is 0xC3 (Ã) and next is available
    if (*input == 0x00C3 && *(input + 1)) {
      // Get the next character
      wchar_t next = *(input + 1);
      
      // Calculate the actual character
      // e.g. For Ã¥ (0xC3 0xA5) -> å (0xE5)
      wchar_t combined = 0;
      if (next == 0x00A5) {  // å
        combined = 0x00E5;
      } else if (next == 0x00A4) {  // ä
        combined = 0x00E4;
      } else if (next == 0x00B6) {  // ö
        combined = 0x00F6;
      } else if (next == 0x0085) {  // Å
        combined = 0x00C5;
      } else if (next == 0x0084) {  // Ä
        combined = 0x00C4;
      } else if (next == 0x0096) {  // Ö
        combined = 0x00D6;
      }

      if (combined) {
        *output++ = combined;
        input += 2;  // Skip both characters
        continue;
      }
    }
    
    // Copy character as-is if no combination is needed
    *output++ = *input++;
  }
  *output = L'\0';
}

void test_combine_unicode() {
  wchar_t wbuf[BUFFER];
  wchar_t result[BUFFER];

  // Test combining unicode
  char str[] = "fjÃ¤rde";
  mbstowcs(wbuf, str, sizeof(str));
  combine_unicode(wbuf, result);
  print_unicode(wbuf);
  print_unicode(result);
  print_method3(result);
}

void test_add() {
  node *root = NULL;
  wchar_t str[30] = L"abcdefghijklmnopqrstuvwxyzåäö";
  root = add(root, str);
}

/* ------------------------------ Print methods ----------------------------- */

// Print the hex values of a string in UTF-8
void print_unicode(wchar_t *wstr) {
  while (*wstr) {
    printf("U+%04x ", *wstr);
    wstr++;
  }
  printf("\n");
}

// Not working
// wprintf
void print_method1(wchar_t *wstr) {
  wprintf(L"Method 1 (wprintf): %ls\n", wstr);
}

// Not working
// putwchar
void print_method2(wchar_t *wstr) {
  printf("Method 2 (putwchar): ");
  while (*wstr) {
    putwchar(*wstr);
    wstr++;
  }
  printf("\n");
}

// printf with %lc
void print_method3(wchar_t *wstr) {
  // printf("Method 3 (printf %%lc): ");
  while (*wstr) {
    printf("%lc", *wstr);
    wstr++;
  }
  printf("\n");
}

// Initialize word list
word_list* create_word_list(int initial_capacity) {
  word_list* list = (word_list*)malloc(sizeof(word_list));
  list->words = (wchar_t**)malloc(initial_capacity * sizeof(wchar_t*));
  list->count = 0;
  list->capacity = initial_capacity;
  return list;
}

// Add word to list
void add_word(word_list* list, wchar_t* word) {
  if (list->count >= list->capacity) {
    list->capacity *= 2;
    list->words = (wchar_t**)realloc(list->words, list->capacity * sizeof(wchar_t*));
  }
  int len = wcslen(word);
  list->words[list->count] = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
  wcscpy(list->words[list->count], word);
  list->count++;
}

void collect(node* current, const char* keys, int key_pos, 
                  wchar_t* prefix, int prefix_pos, word_list* results) {
  if (current == NULL) return;
  
  // If we've processed all keys, check if we have a valid word
  if (keys[key_pos] == '\0') {
    if (current->valid) {
      prefix[prefix_pos] = L'\0';
      add_word(results, prefix);
    }
    return;
  }
  
  // Convert key to index (2->1, 3->2, etc.)
  int key_idx = keys[key_pos] - '1';
  if (key_idx < 0 || key_idx > 8) return;  // Invalid key
  
  // Get the range of characters for this key
  int start = key_start[key_idx];
  int len = key_length[key_idx];
  
  // Try each possible character for this key
  for (int i = 0; i < len; i++) {
    int char_idx = start + i;
    if (current->next[char_idx] != NULL) {
      // Add character to prefix
      prefix[prefix_pos] = L'a' + char_idx;
      // Special handling for Swedish characters
      if (char_idx == 26) prefix[prefix_pos] = L'å';
      else if (char_idx == 27) prefix[prefix_pos] = L'ä';
      else if (char_idx == 28) prefix[prefix_pos] = L'ö';
      
      // Recurse with next key
      collect(current->next[char_idx], keys, key_pos + 1, 
              prefix, prefix_pos + 1, results);
    }
  }
}

wchar_t** decode(trie* tr, const char* sequence, int* count) {
  if (tr == NULL || tr->root == NULL || sequence == NULL) {
    *count = 0;
    printf("Invalid trie or sequence\n");
    return NULL;
  }
  
  word_list* results = create_word_list(100);  // Start with space for 100 words
  wchar_t prefix[256] = {0};  // Buffer for building words
  
  collect(tr->root, sequence, 0, prefix, 0, results);
  
  *count = results->count;
  wchar_t** words = results->words;
  free(results);  // Free the list structure but keep the words array
  
  return words;
}

void free_decoded(wchar_t** words, int count) {
  for (int i = 0; i < count; i++) {
    free(words[i]);
  }
  free(words);
}