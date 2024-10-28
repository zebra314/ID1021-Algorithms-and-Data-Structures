#include "hash.h"

#define BUFFER 200
#define AREAS 10000
struct timespec t_start, t_stop;

/* -------------------------------------------------------------------------- */
/*                                Init function                               */
/* -------------------------------------------------------------------------- */

codes *read_postcodes(char *file) {

  // Allocate memory for the struct
  codes *postnr = (codes*)malloc(sizeof(codes));
  area *areas = (area*)malloc(sizeof(area)*AREAS);

  // Set up the file pointer and the buffer
  FILE *fptr = fopen(file, "r");
  if (fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", file);
    free(postnr);
    free(areas);
    return NULL;
  }

  int k = 0;
  char *lineptr = malloc(sizeof(char)*BUFFER);
  size_t n = BUFFER;

  // Open a file in read mode
  while((k < AREAS) && (getline(&lineptr, &n, fptr) > 0)) {
    area a;

    // Separate each field in the line
    char *zip_token = strtok(lineptr, ",");
    char *name_token = strtok(NULL, ",");
    char *pop_token = strtok(NULL, ",");

    // Allocate memory for each field and copy contents
    a.zip_char = strdup(zip_token);   // Allocate and copy zip
    a.zip_int = zip_to_int(a.zip_char);
    a.name = strdup(name_token); // Allocate and copy name
    a.pop = atoi(pop_token);     // Convert pop to integer

    // update the array of areas
    areas[k++] = a;
  }
  fclose(fptr);

  postnr->areas = areas;
  postnr->n = k;

  return postnr;
}

codes *init_direct(codes *postnr) {
  postnr->areas_direct = (area**)malloc(sizeof(area*)*100000);
  for(int i = 0; i < postnr->n; i++) {
    int zip = zip_to_int(postnr->areas[i].zip_char);
    postnr->areas_direct[zip] = &postnr->areas[i];
  }
  return postnr;
}

// Initialize a hash table with a given size
// using bucket to handle collisions
codes* init_hash_table(codes *postnr, int size) {
  postnr->buckets = (bucket**)malloc(sizeof(bucket*)*size);
  postnr->size = size;
  for(int i = 0; i < postnr->n; i++) {
    insert_bucket(postnr, &postnr->areas[i]);
  }
  return postnr;
}

// Insert into bucket
void insert_bucket(codes *postnr, area *a) {
  int index = hash(a->zip_int, postnr->size);
  
  // If bucket doesn't exist, create it
  if(postnr->buckets[index] == NULL) {
    postnr->buckets[index] = malloc(sizeof(bucket));
    postnr->buckets[index]->capacity = 1;
    postnr->buckets[index]->size = 0;
    postnr->buckets[index]->areas = malloc(sizeof(area));
  }
  
  // If bucket is full, resize it
  bucket *b = postnr->buckets[index];
  if(b->size >= b->capacity) {
    b->capacity *= 2;
    b->areas = realloc(b->areas, b->capacity * sizeof(area));
  }
  
  // Add the area
  b->areas[b->size] = *a;
  b->size++;
}

/* -------------------------------------------------------------------------- */
/*                              Search functions                              */
/* -------------------------------------------------------------------------- */

// Linear search with char zip
area* linear_search_char(codes *postnr, const char *zip) {
  for (int i = 0; i < postnr->n; i++) {
    if (strcmp(postnr->areas[i].zip_char, zip) == 0) {
      return &postnr->areas[i];
    }
  }
  return NULL;
}

// Linear search with int zip
area* linear_search_int(codes *postnr, int zip) {
  for (int i = 0; i < postnr->n; i++) {
    if (postnr->areas[i].zip_int == zip) {
      return &postnr->areas[i];
    }
  }
  return NULL;
}

// Binary search with char zip
area* binary_search_char(codes *postnr, const char *zip) {
  int left = 0;
  int right = postnr->n - 1;
  
  while (left <= right) {
    int mid = (left + right) / 2;
    int cmp = strcmp(postnr->areas[mid].zip_char, zip);
    
    if (cmp == 0) {
      return &postnr->areas[mid];
    }
    if (cmp < 0) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return NULL;
}

// Binary search with int zip
area* binary_search_int(codes *postnr, int zip) {
  int left = 0;
  int right = postnr->n - 1;
  
  while (left <= right) {
    int mid = (left + right) / 2;
    int cmp = postnr->areas[mid].zip_int - zip;
    
    if (cmp == 0) {
      return &postnr->areas[mid];
    }
    if (cmp < 0) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return NULL;
}

// Lookup function
area* direct_lookup(codes *postnr, int zip) {
  if(zip >= 0 && zip <= 100000) {
    return postnr->areas_direct[zip];
  }
  return NULL;
}

// Lookup function in hash table with buckets
area* hash_lookup(codes *postnr, int zip) {
  int index = hash(zip, postnr->size);
  bucket *b = postnr->buckets[index];
  
  for(int i = 0; i < b->size; i++) {
    if(b->areas[i].zip_int == zip) {
      return &b->areas[i];
    }
  }
  return NULL;
}

/* -------------------------------------------------------------------------- */
/*                               Tool functions                               */
/* -------------------------------------------------------------------------- */

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
         (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

// Zip function, transform char zip to int zip
codes* new_zip(codes *postnr) {
  for(int i = 0; i<postnr->n; i++) {
    char *zip = postnr->areas[i].zip_char;
    postnr->areas[i].zip_int = atoi(zip)*100 + atoi(zip+3);
  }
  return postnr;
}

// Convert string zip code to integer (e.g., "111 15" -> 11115)
int zip_to_int(const char *zip) {
  int num = 0;
  for(int i = 0; zip[i] != '\0'; i++) {
    if(zip[i] >= '0' && zip[i] <= '9') {
      num = num * 10 + (zip[i] - '0');
    }
  }
  return num;
}

// Count collisions as provided in the assignment
void count_collisions(codes *postnr) {
  int mx = 20;
  int *cols = calloc(mx, sizeof(int));
  
  // Count elements in each bucket
  for(int i = 0; i < postnr->size; i++) {
    if(postnr->buckets[i] != NULL) {
      int count = postnr->buckets[i]->size;
      if(count < mx) {
        cols[count]++;
      }
    } else {
      cols[0]++;
    }
  }
  
  // Print results
  printf("Collisions:\n");
  for(int i = 1; i < mx; i++) {
    printf("%6d ", cols[i]);
  }
  printf("\n");

  free(cols);
}

// Hash function
int hash(int zip, int table_size) {
  return zip % table_size;
}

/* -------------------------------------------------------------------------- */
/*                       Test functions for benchmarking                      */
/* -------------------------------------------------------------------------- */

// Benchmark searches for "111 15" and "984 99"
void test1(codes *postnr) {
  area *a;

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = linear_search_char(postnr, "111 15");
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Linear search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = binary_search_char(postnr, "111 15");
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Binary search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = linear_search_char(postnr, "984 99");
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Linear search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = binary_search_char(postnr, "984 99");
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Binary search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  // Clean up
  for (int i = 0; i < postnr->n; i++) {
    free(postnr->areas[i].name);
    free(postnr->areas[i].zip_char);
  }
  free(postnr->areas);
  free(postnr);
}

// Benchmark searches for "111 15" and "984 99"
// with new zip function
void test2(codes *postnr) {
  postnr = new_zip(postnr);

  area *a;

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = linear_search_int(postnr, 11115);
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Linear search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = binary_search_int(postnr, 11115);
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Binary search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = linear_search_int(postnr, 98499);
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Linear search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = binary_search_int(postnr, 98499);
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Binary search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  // Clean up
  for (int i = 0; i < postnr->n; i++) {
    free(postnr->areas[i].name);
    free(postnr->areas[i].zip_char);
  }
  free(postnr->areas);
  free(postnr);
}

// Implement the direct indexing method using zip codes as array indices
// Compare the performance of direct indexing with binary search
// for the zip codes "111 15" and "984 99"
void test3(codes *postnr) {
  postnr = new_zip(postnr);
  postnr = init_direct(postnr);

  area *a;

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = direct_lookup(postnr, 11115);
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Direct lookup: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = binary_search_int(postnr, 11115);
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Binary search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = direct_lookup(postnr, 98499);
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Direct lookup: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = binary_search_int(postnr, 98499);
    if (a == NULL) {
      fprintf(stderr, "Not found\n");
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Binary search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  // Clean up
  for (int i = 0; i < postnr->n; i++) {
    free(postnr->areas[i].name);
    free(postnr->areas[i].zip_char);
  }
  free(postnr->areas);
  free(postnr);
}

// Implement the hash table method using zip codes as keys and using buckets
void test4(codes *postnr) {
  postnr = init_hash_table(postnr, 14000);
  count_collisions(postnr);

  // Clean up
  for (int i = 0; i < postnr->n; i++) {
    free(postnr->areas[i].name);
    free(postnr->areas[i].zip_char);
  }
  free(postnr->areas);
  free(postnr);
}