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

typedef struct bucket {
  area *areas;        // Dynamic array of areas
  int size;           // Current size of bucket
  int capacity;       // Capacity of bucket
} bucket;

typedef struct codes {
  area *areas;
  area **areas_direct; // Array of pointers to areas
  bucket **buckets;    // Array of pointers to buckets
  int n;               // Number of areas
  int size;            // Size of hash table
} codes;

// Init
codes *read_postcodes(char *file);
codes *init_direct(codes *postnr);
codes* init_hash_table(codes *postnr, int size);
void insert_bucket(codes *postnr, area *a);

// Search
area* linear_search_char(codes *postnr, const char *zip);
area* binary_search_char(codes *postnr, const char *zip);
area* linear_search_int(codes *postnr, int zip);
area* binary_search_int(codes *postnr, int zip);
area* direct_lookup(codes *postnr, int zip);

// Tools
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);
codes* new_zip(codes *postnr);
int zip_to_int(const char *zip);
void count_collisions(codes *postnr);
int hash(int zip, int table_size);

// Tests
void test1(codes *postnr);
void test2(codes *postnr);
void test3(codes *postnr);
void test4(codes *postnr);

#endif // HASH_H