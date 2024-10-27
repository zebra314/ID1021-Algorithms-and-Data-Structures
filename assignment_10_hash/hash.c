#include "hash.h"

#define BUFFER 200
#define AREAS 10000

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
    char *copy = (char*)malloc(sizeof(char)*n);
    strcpy(copy, lineptr);

    // divide the copy into three strings
    area a;
    a.zip = strtok(copy, ",");
    a.name = strtok(NULL, ",");
    a.pop = atoi(strtok(NULL, ","));

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
    int index = atoi(postnr->areas[i].zip)%mod;
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

// Linear search
area* linear_search(codes *postnr, const char *zip) {
  for (int i = 0; i < postnr->n; i++) {
    if (strcmp(postnr->areas[i].zip, zip) == 0) {
      return &postnr->areas[i];
    }
  }
  return NULL;
}

// Binary search
area* binary_search(codes *postnr, const char *zip) {
  int left = 0;
  int right = postnr->n - 1;
  
  while (left <= right) {
    int mid = (left + right) / 2;
    int cmp = strcmp(postnr->areas[mid].zip, zip);
    
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

// new zip function
codes* new_zip(codes *postnr) {
  for(int i = 0; i<postnr->n; i++) {
    char *zip = postnr->areas[i].zip;
    postnr->areas[i].zip = atoi(zip)*100 + atoi(zip+3);
  }
  return postnr;
}

// Benchmark searches for "111 15" and "984 99"
void test1(codes *postnr) {
  area *a;

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = linear_search(postnr, "111 15");
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Linear search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = binary_search(postnr, "111 15");
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Binary search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = linear_search(postnr, "984 99");
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Linear search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  clock_gettime(CLOCK_REALTIME, &t_start);
  for(int i = 0; i < 1000; i++) {
    a = binary_search(postnr, "984 99");
  }
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Binary search: %ld ns\n", nano_seconds(&t_start, &t_stop)/1000);

  // Clean up
  for (int i = 0; i < postnr->n; i++) {
    free(postnr->areas[i].name);
    free(postnr->areas[i].zip);
  }
  free(postnr->areas);
  free(postnr);

  return 0;
}

// Benchmark searches for "111 15" and "984 99"
// with new zip function
void test2(codes *postnr) {
  postnr = new_zip(postnr);

  area *a;

  clock_gettime(CLOCK_REALTIME, &t_start);
  a = linear_search(postnr, "111 15");
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Linear search: %ld ns\n", nano_seconds(&t_start, &t_stop));

  clock_gettime(CLOCK_REALTIME, &t_start);
  a = binary_search(postnr, "111 15");
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Binary search: %ld ns\n", nano_seconds(&t_start, &t_stop));

  clock_gettime(CLOCK_REALTIME, &t_start);
  a = linear_search(postnr, "984 99");
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Linear search: %ld ns\n", nano_seconds(&t_start, &t_stop));

  clock_gettime(CLOCK_REALTIME, &t_start);
  a = binary_search(postnr, "984 99");
  clock_gettime(CLOCK_REALTIME, &t_stop);
  printf("Binary search: %ld ns\n", nano_seconds(&t_start, &t_stop));

}