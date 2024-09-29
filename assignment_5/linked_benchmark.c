#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// Link list data structure
typedef struct cell {
  int value;
  struct cell *tail;
} cell;

typedef struct linked {
  cell *first;
} linked;

// Linked list functions
linked *linked_create();
linked *linked_init(int n);
void linked_free(linked *lnk);
void linked_add(linked *lnk, int item);
void linked_append(linked *a, linked *b);
int linked_length(linked *lnk);
bool linked_find(linked *lnk, int item);
void linked_remove(linked *lnk, int item);

// Tool functions
struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);

int main() {
  linked *a = linked_init(1000000);
  linked *b = linked_init(1000000);
  linked_append(a, b);
  linked_free(a);
  linked_free(b);
  return 0;
}

linked *linked_create() {
  linked *new = (linked*)malloc(sizeof(linked));
  new->first = NULL;
  return new;
}

linked *linked_init(int n) {
  linked *a = linked_create();
  for (int i = 0; i < n; i++) {
    linked_add(a, i);
  }
  return a;
}

void linked_free(linked *lnk) {
  cell *curr = lnk->first;
  while (curr != NULL) {
    cell *tmp = curr->tail;
    free(curr);
    curr = tmp;
  }
  free(lnk);
}

// Add the item as the first cell in the list.
void linked_add(linked *lnk, int item) {
  cell *new = (cell*)malloc(sizeof(cell));
  new->value = item;
  new->tail = lnk->first;
  lnk->first = new;
}

// Add linked list a to the end of linked-list b.
void linked_append(linked *a, linked *b) {
  cell *curr = a->first;
  cell *prv = NULL;

  // Find the last cell in the first list(a)
  while(curr != NULL) {
    prv = curr;
    curr = curr->tail;
  }

  // Set the tail of the last cell in the first list(a) 
  // to the first cell in the second list(b)
  if (prv != NULL) {
    prv->tail = b->first;
  } else {
    a->first = b->first;
  }
}

// Return the length of the list.
int linked_length(linked *lnk) {
  int len = 0;
  cell *curr = lnk->first;
  while (curr != NULL) {
    len++;
    curr = curr->tail;
  }
  return len;
}

// Return true or false depending on if the item can be found in the list.
bool linked_find(linked *lnk, int item) {
  cell *curr = lnk->first;
  while (curr != NULL) {
    if (curr->value == item) {
      return true;
    }
    curr = curr->tail;
  }
  return false;
}

// Remove the item if it exists in the list.
void linked_remove(linked *lnk, int item) {
  cell *curr = lnk->first;
  cell *prv = NULL;
  while (curr != NULL) {
    if (curr->value != item) {
      prv = curr;
      curr = curr->tail;
    } else {
      if(prv != NULL) {
        // Link the previous cell to the next cell.
        prv->tail = curr->tail;
      } else {
        // If the first cell is the one to be removed,
        lnk->first = curr->tail;
      }
      cell *tmp = curr->tail;
      free(curr);
      curr = tmp;
    }
  }
}

// Return the time in nano seconds.
long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
         (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}