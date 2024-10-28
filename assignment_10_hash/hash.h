#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct area {
  char *name;
  char *zip_char;
  int zip_int;
  int pop;
} area;

typedef struct codes {
  area *areas;
  int n;
} codes;

// Benchmark
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);

// Search
area* linear_search_char(codes *postnr, const char *zip);
area* binary_search_char(codes *postnr, const char *zip);
area* linear_search_int(codes *postnr, int zip);
area* binary_search_int(codes *postnr, int zip);


// Hash
codes *read_postcodes(char *file);
codes* new_zip(codes *postnr);
void collisions(codes *postnr, int mod);
void test1(codes *postnr);
void test2(codes *postnr);

#endif // HASH_H