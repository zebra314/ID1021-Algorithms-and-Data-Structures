#include "dijkstra.h"

Map *graph(char *file) {
  // Open adressing, linear probing
  Map *trains = (Map*)malloc(sizeof(Map));
  City *cities = (City*)malloc(sizeof(City)*MOD); // Hash table
  trains->cities = cities;
  trains->size = 0;
  trains->capacity = MOD;

  // Open a file in read mode
  FILE *fptr = fopen(file, "r");
  if (fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", file);
    free(trains);
    free(cities);
    return NULL;
  }

  // Extract the data from the file
  char *lineptr = malloc(sizeof(char)*BUFFER);
  size_t n = BUFFER;
  while(getline(&lineptr, &n, fptr) > 0) {
    char *copy = (char*)malloc(sizeof(char)*(strlen(lineptr)+1));
    strcpy(copy, lineptr);

    // Divide the copy into the three parts
    City *src = lookup_city(trains,strtok(copy, ","));
    City *dst = lookup_city(trains, strtok(NULL, ","));
    int time = atoi(strtok(NULL, ","));

    // Add the connection
    connect(src, dst, time);
  }
  fclose(fptr);

  return trains;
}

// Return the pointer to the city with the name, if it does not exist, create it
City *lookup_city(Map *map, char *name) {
  int index = hash(name, map->capacity);
  while(map->cities[index].name != NULL && strcmp(map->cities[index].name, name) != 0) {
    index = (index + 1) % map->capacity;
  }

  if(map->cities[index].name == NULL) {
    City *c = (City*)malloc(sizeof(City));
    c->name = strdup(name);
    c->id = map->size;
    c->connections = (Connection*)malloc(sizeof(Connection)*MOD);
    c->size = 0;
    c->capacity = MOD;
    map->cities[index] = *c;
    map->size++;
  }
  return &map->cities[index];
}

// Set up connection information at both src and dst cities
void connect(City *src, City *dst, int time) {
  Connection *c = (Connection*)malloc(sizeof(Connection));
  int index; // For the hash function
  c->time = time;

  // Set connection at city src
  c->dst = dst;
  if(src->size >= src->capacity) {
    src->capacity *= 2;
    src->connections = realloc(src->connections, src->capacity * sizeof(Connection));
  }
  src->connections[src->size] = *c;
  src->size++;

  // Set connection at city dst
  c->dst = src;
  if(dst->size >= dst->capacity) {
    dst->capacity *= 2;
    dst->connections = realloc(dst->connections, dst->capacity * sizeof(Connection));
  }
  dst->connections[dst->size] = *c;
  dst->size++;
}

// Transform city name to index
int hash(char *name, int mod) {
  int h = 0;
  int i = 0;
  unsigned char c = 0;

  while((c = name[i]) != 0) {
    h = (h*31 + c) % mod;
    i++;
  }
  return h;
}

/* -------------------------------------------------------------------------- */
/*                           Prior Queue operations                           */
/* -------------------------------------------------------------------------- */

PriorityQueue *new_priority_queue() {
  PriorityQueue *pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
  pq->paths = (Path**)malloc(sizeof(Path*)*MOD);
  pq->size = 0;
  pq->capacity = MOD;
  return pq;
}

void push(PriorityQueue *pq, Path *p) {
  if(pq->size >= pq->capacity) {
    pq->capacity *= 2;
    pq->paths = realloc(pq->paths, pq->capacity * sizeof(Path*));
  }
  pq->paths[pq->size] = p;
  pq->size++;
}

Path *pop(PriorityQueue *pq) {
  if(pq->size == 0) {
    return NULL;
  }

  // Find the path with the minimum time
  int min = 0;
  for(int i = 1; i < pq->size; i++) {
    if(pq->paths[i]->total_time < pq->paths[min]->total_time) {
      min = i;
    }
  }

  // Remove the path from the queue
  Path *p = pq->paths[min];
  pq->size--;
  pq->paths[min] = pq->paths[pq->size];
  return p;
}

/* -------------------------------------------------------------------------- */
/*                                   Search                                   */
/* -------------------------------------------------------------------------- */

Path *dijkstra(Map *map, City *from, City *to) {
  PriorityQueue *pq = new_priority_queue();
  Path **result = (Path**)malloc(map->size * sizeof(Path*));
  for(int i = 0; i < map->size; i++) {
    result[i] = (Path*)malloc(sizeof(Path));      
    result[i]->city = NULL;
    result[i]->total_time = 0;
    result[i]->prev = NULL;
  }

  result[from->id]->city = from;
  result[from->id]->total_time = 0;
  push(pq, result[from->id]);

  while(pq->size > 0) {
    Path *p = pop(pq);
    City *c = p->city;

    if(c == to) {
      // printf("Found\n");
      // Check the amount of nodes in the "done" array
      int done_count = 0;
      for (int i = 0; i < map->size; i++) {
        if (result[i]->city != NULL) {
          done_count++;
        }
      }
      printf("%d,", done_count);
      return p;
    }

    // Check all connections
    result[c->id] = p;
    for(int i = 0; i < c->size; i++) {
      Connection *nxt = &c->connections[i];

      // If the city is already in the path, skip it
      // or if the time is not better, skip it        
      if(result[nxt->dst->id]->city != NULL || 
          (result[nxt->dst->id]->total_time >= p->total_time + nxt->time)) {
        continue;
      }

      // Add the city to the queue
      Path *new = (Path*)malloc(sizeof(Path));
      new->city = nxt->dst;
      new->total_time = p->total_time + nxt->time;
      new->prev = p;
      push(pq, new);
    }
  }
  printf("No path found\n");
  return NULL;
}

void path_info(Path *p) {
  if(p == NULL) {
    printf("No path found\n");
    return;
  }
  printf("Path:\n");
  print_path(p);
}

void print_path(Path *p) {
  if(p == NULL) 
    return;
  print_path(p->prev);
  printf("%s\n", p->city->name);
  printf("Time: %d\n", p->total_time);
  printf("\n");
}

void free_path(Path *p) {
  if(p == NULL)
    return;
  free_path(p->prev);
  free(p);
}

/* -------------------------------------------------------------------------- */
/*                                    Test                                    */
/* -------------------------------------------------------------------------- */

// Benchmarking
struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
         (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

void test1() {
  Map *map = graph("europe.csv");
  City *from = lookup_city(map, "Malmö");
  City *to = lookup_city(map, "Kiruna");

  clock_gettime(CLOCK_REALTIME, &t_start);
  Path *p = dijkstra(map, from, to);
  clock_gettime(CLOCK_REALTIME, &t_stop);

  if(p == NULL) {
    printf("No path found\n");
  } else {
    print_path(p);
  }

  long ns = nano_seconds(&t_start, &t_stop);
  printf("Compute Time: %ld ns\n", ns);
}

void test2() {
  struct timespec t_start, t_stop;
  long ns;

  Map *map = graph("europe.csv");
  City *start_city = lookup_city(map, "Malmö");

  // char *cities[] = {"Kiruna", "Stockholm", "Göteborg", "Uppsala", "Örebro", "Karlstad", "Sundsvall",
  //                     "Umeå", "Luleå", "Gävle", "Linköping", "Helsingborg"};
  char *cities[] = {
    "Stockholm", "Södertälje", "Norrköping", "Linköping", "Mjölby",
    "Nässjö", "Alvesta", "Hässleholm", "Lund", "Malmö",
    "Göteborg", "Varberg", "Halmstad", "Åstorp", "Skövde",
    "Herrljunga", "Falköping", "Jönköping", "Värnamo", "Emmaboda",
    "Kalmar", "Kristianstad", "Karlskrona", "Hallsberg", "Örebro",
    "Arboga", "Västerås", "Uppsala", "Gävle", "Sundsvall"
  };
  int num_cities = sizeof(cities) / sizeof(cities[0]);

  // printf("Benchmark results for starting city: %s\n", start_city->name);
  // printf("Destination City | Compute Time (ns) | Nodes in Done Array\n");
  // printf("---------------------------------------------------------\n");

  for (int i = 0; i < num_cities; i++) {
    City *destination = lookup_city(map, cities[i]);
    if (destination == NULL) {
      printf("City not found: %s\n", cities[i]);
      continue;
    }

    clock_gettime(CLOCK_REALTIME, &t_start);
    Path *path = dijkstra(map, start_city, destination);
    clock_gettime(CLOCK_REALTIME, &t_stop);

    ns = nano_seconds(&t_start, &t_stop);

    if (path == NULL) {
      printf("%s | No path found\n", destination->name);
    } else {
      printf("%s,%ld\n", destination->name, ns);
      free_path(path);  // Free memory for the path after use if needed
    }
  }
}

void check_connection_avg(Map *map) {
  int total_connections = 0;
  for (int i = 0; i < map->size; i++) {
    total_connections += map->cities[i].size;
  }
  printf("Average connections per city: %f\n", (float)total_connections / map->size);
}
