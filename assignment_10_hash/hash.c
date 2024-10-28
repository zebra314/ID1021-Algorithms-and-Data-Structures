#include "hash.h"

#define BUFFER 200
#define AREAS 10000
struct timespec t_start, t_stop;

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

void collisions(codes *postnr, int mod) {
  int mx = 20;
  int data[mod];
  int cols[mx];

  for(int i = 0; i < mod; i++) {
    data[i] = 0;
  }

  for(int i = 0; i < mx; i++) {
    cols[i] = 0;
  }

  for (int i = 0; i < postnr->n; i++) {
    int index = atoi(postnr->areas[i].zip_char)%mod;
    data[index]++;
  }

  int sum = 0;
  for(int i = 0; i < mod; i++) {
    sum += data[i];
    if (data[i] < mx)
      cols[data[i]]++;
  }

  printf("%d (%d) : ", mod, sum);
  for (int i = 1; i < mx; i++) {
    printf("%6d ",cols[i]);
  }
  printf("\n");
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

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
         (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

// Zip function
// Transform char zip to int zip
codes* new_zip(codes *postnr) {
  for(int i = 0; i<postnr->n; i++) {
    char *zip = postnr->areas[i].zip_char;
    postnr->areas[i].zip_int = atoi(zip)*100 + atoi(zip+3);
  }
  return postnr;
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