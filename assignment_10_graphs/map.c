#include "map.h"

/* -------------------------------------------------------------------------- */
/*                                    Init                                    */
/* -------------------------------------------------------------------------- */

#define MOD 100
#define BUFFER 200

map *graph(char *file) {
  // Open adressing, linear probing
  map *trains = (map*)malloc(sizeof(map));
  city *cities = (city*)malloc(sizeof(city)*MOD);
  trains->cities = cities;
  trains->n = 0;
  trains->capacity = MOD;

  // Open a file in read mode
  FILE *fptr = fopen(file, "r");
  if (fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", file);
    free(trains);
    free(cities);
    return NULL;
  }

  char *lineptr = malloc(sizeof(char)*BUFFER);
  size_t n = BUFFER;
  while(getline(&lineptr, &n, fptr) > 0) {
    char *copy = (char*)malloc(sizeof(char)*(strlen(lineptr)+1));
    strcpy(copy, lineptr);

    // divide the copy into the three parts
    city *src = lookup_city(trains,strtok(copy, ","));
    city *dst = lookup_city(trains, strtok(NULL, ","));
    int dist = atoi(strtok(NULL, ","));

    // add the connection
    connect(src, dst, dist);
  }
  fclose(fptr);

  return trains;
}

city *lookup_city(map *map, char *name) {
  int index = hash(name, map->capacity);
  while(map->cities[index].name != NULL && strcmp(map->cities[index].name, name) != 0) {
    index = (index + 1) % map->capacity;
  }

  if(map->cities[index].name == NULL) {
    city *c = (city*)malloc(sizeof(city));
    c->name = strdup(name);
    c->connections = (connection*)malloc(sizeof(connection)*MOD);
    c->recent_connections = (connection*)malloc(sizeof(connection)*MOD);
    c->n = 0;
    c->capacity = MOD;
    map->cities[index] = *c;
    map->n++;
  }
  return &map->cities[index];
}

// Set up connection information at both src and dst cities
void connect(city *src, city *dst, int time) {
  connection *c = (connection*)malloc(sizeof(connection));
  int index; // For the hash function
  c->time = time;

  // Set connection at city src
  c->dst = dst;
  if(src->n >= src->capacity) {
    src->capacity *= 2;
    src->connections = realloc(src->connections, src->capacity * sizeof(connection));
  }

  index = hash(dst->name, src->capacity);
  while(src->connections[index].dst != NULL) {
    index = (index + 1) % src->capacity;
  }
  src->connections[index] = *c;
  src->recent_connections[src->n] = *c;
  src->n++;

  // Set connection at city dst
  c->dst = src;
  if(dst->n >= dst->capacity) {
    dst->capacity *= 2;
    dst->connections = realloc(dst->connections, dst->capacity * sizeof(connection));
  }

  index = hash(src->name, dst->capacity);
  while(dst->connections[index].dst != NULL) {
    index = (index + 1) % dst->capacity;
  }
  dst->connections[index] = *c;
  dst->recent_connections[dst->n] = *c;
  dst->n++;
}

// Hash function, transform city name to index
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
/*                                   Search                                   */
/* -------------------------------------------------------------------------- */

int shortest(city *from, city *to, int left, path *path) {
  if (from == to) {
    path->cities[path->n] = *from;
    path->n++;
    return 0;
  }

  int sofar = -1;
  for(int i = 0; i < from->n; i++) {
    connection *c = &from->recent_connections[i];
    if (c->time <= left) {
      left -= c->time;
      int d = shortest(c->dst, to, left, path);
      if (d >= 0 && ((sofar == -1) || (d + c->time) < sofar)) {
        sofar = (d + c->time);
        path->cities[path->n] = *from;
      }
    }
  }
  path->n++;
  return sofar;
}