#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct area {
  char *name;
  char *zip;
  int pop;
} area;

typedef struct codes {
  area *areas;
  int n;
} codes;

// Benchmark
struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);

// Search
area* linear_search(codes *postnr, const char *zip);

// Hash
codes *read_postcodes(char *file);
void collisions(codes *postnr, int mod);

#endif // HASH_H