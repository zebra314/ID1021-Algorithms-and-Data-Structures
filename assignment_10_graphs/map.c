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

// Return the pointer to the city with the name, if it does not exist, create it
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

// Return the connection between src and dst, if it does not exist, return NULL
connection *lookup_connection(city *src, city *dst) {
  for(int i = 0; i < src->n; i++) {
    if(src->connections[i].dst == dst && src->connections[i].dst != NULL) {
      return &src->connections[i];
    }
  }
  return NULL;
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

path *new_path() {
  path *p = (path*)malloc(sizeof(path));
  p->cities = (city*)malloc(sizeof(city)*MOD);
  p->times = (int*)malloc(sizeof(int)*MOD);
  p->n = 0;
  return p;
}

/* -------------------------------------------------------------------------- */
/*                                   Search                                   */
/* -------------------------------------------------------------------------- */

int shortest(city *from, city *to, int left, path *path) {
  // Reached destination
  if (from == to) {
    path->n = 0;
    path->cities[path->n] = *from;
    path->times[path->n] = 0;
    path->n++;
    return 0;
  }

  int sofar = -1; // Time to destination
  int update = 0; // Update path

  // Check all connections
  for(int i = 0; i < from->n; i++) {
    connection *c = &from->recent_connections[i];

    // If there is time left, try to reach the destination
    if (c->time <= left) {
      left -= c->time;

      // Recursively call the function
      // check children
      int d = shortest(c->dst, to, left, path);

      // If the destination is reached or the time is less than the previous time
      // update the path
      if (d >= 0 && ((sofar == -1) || (d + c->time) < sofar)) {
        sofar = (d + c->time);
        path->cities[path->n] = *from;
        path->times[path->n] = c->time;
        update = 1;
      }
    }
  }

  // After check all connections, if the path is updated, 
  // increase the number of cities
  if (update)
    path->n++;

  return sofar;
}

int shortest_path(city *from, city *to, city **path, int k) {
  if (from == to) {
    return 0;
  }
  int sofar = -1;
  // Check all connections
  for(int i = 0; i < from->n; i++) {
    connection *nxt = &from->recent_connections[i];
    if (!loop(path, k, nxt->dst)) {
      // Add the city to the path
      path[k] = nxt->dst;
      int d = shortest_path(nxt->dst, to, path, k+1);
      if (d >= 0 && ((sofar == -1 ) || (d + nxt->time) < sofar)) {
        sofar = (d + nxt->time);
      }
    }
  }
  return sofar;
}

// Returns 1 if the city is already in the path, 0 otherwise
int loop(city **path, int k, city *dst) {
  for (int i = 0; i < k; i++) {
    if (path[i] == dst) {
      return 1;  // City found in path, loop detected
    }
  }
  return 0;  // No loop detected
}