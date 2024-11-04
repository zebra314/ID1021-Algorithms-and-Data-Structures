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

  index = hash(dst->name, src->capacity);
  while(src->connections[index].dst != NULL) {
    index = (index + 1) % src->capacity;
  }
  src->connections[index] = *c;
  src->size++;

  // Set connection at city dst
  c->dst = src;
  if(dst->size >= dst->capacity) {
    dst->capacity *= 2;
    dst->connections = realloc(dst->connections, dst->capacity * sizeof(Connection));
  }

  index = hash(src->name, dst->capacity);
  while(dst->connections[index].dst != NULL) {
    index = (index + 1) % dst->capacity;
  }
  dst->connections[index] = *c;
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

/* --------------------------------- Search --------------------------------- */

Path *dijsktra(Map *map, City *from, City *to) {
  PriorityQueue *pq = new_priority_queue();
  Path *p = (Path*)malloc(sizeof(Path));
  p->city = from;
  p->total_time = 0;
  p->prev = NULL;
  push(pq, p);

  while(pq->size > 0) {
    Path *p = pop(pq);
    City *c = p->city;

    if(c == to) {
      return p;
    }

    for(int i = 0; i < c->capacity; i++) {
      if(c->connections[i].dst == NULL) {
        continue;
      }
      Connection *nxt = &c->connections[i];
      Path *new = (Path*)malloc(sizeof(Path));
      new->city = nxt->dst;
      new->total_time = p->total_time + nxt->time;
      new->prev = p;
      push(pq, new);
    }
  }
  return NULL;
}