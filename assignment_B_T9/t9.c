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
    print_method3(result);

    kelly->root = add(kelly->root, result);
  }
  fclose(fptr);

  return kelly;
}

node *add(node *nd, wchar_t *rest) {
  if (nd == NULL) {
    nd = (node*)malloc(sizeof(node));
  }

  if (*rest == L'\0') {
    nd->valid = true;
    return nd;
  }

  int c = code((int)*rest);
  if (c == -1) {
    fprintf(stderr, "Invalid character in word: %lc (0x%x)\n", *rest, (unsigned int)*rest);
    return nd;
  }
  nd->next[c] = add(nd->next[c], rest + 1);
}

static int code(wchar_t w) {
  if (w >= L'a' && w <= L'z') return w - L'a';
  if (w >= L'A' && w <= L'Z') return w - L'A';
  
  switch (w) {
    case L'å': case L'Å': return 24;
    case L'ä': case L'Ä': return 25;
    case L'ö': case L'Ö': return 26;
    default:
      return -1;
  }
}

void combine_unicode(const wchar_t *input, wchar_t *output) {
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

// Print the hex values of a string in UTF-8
void print_unicode(const wchar_t *wstr) {
  while (*wstr) {
    printf("U+%04x ", *wstr);
    wstr++;
  }
  printf("\n");
}

// Not working
// wprintf
void print_method1(const wchar_t *wstr) {
  wprintf(L"Method 1 (wprintf): %ls\n", wstr);
}

// Not working
// putwchar
void print_method2(const wchar_t *wstr) {
  printf("Method 2 (putwchar): ");
  while (*wstr) {
    putwchar(*wstr);
    wstr++;
  }
  printf("\n");
}

// printf with %lc
void print_method3(const wchar_t *wstr) {
  // printf("Method 3 (printf %%lc): ");
  while (*wstr) {
    printf("%lc", *wstr);
    wstr++;
  }
  printf("\n");
}
