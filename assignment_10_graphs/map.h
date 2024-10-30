#ifndef _MAP_H_
#define _MAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Elements
typedef struct city city; // Forward declaration

typedef struct connection {
  city *dst;
  int time;
} connection;

typedef struct city {
  char *name;
  connection *connections;        // Dynamic array of connections, Hash table
  connection *recent_connections; // Array of recent connections
  int n;                          // Number of connections
  int capacity;                   // Capacity of connections
} city;

typedef struct map {
  city *cities;            // Dynamic array of cities, Hash table
  int n;                   // Number of cities
  int capacity;            // Capacity of cities
} map;

typedef struct path {
  city *cities;  // Array of cities
  int *times;    // Array of times
  int n;         // Number of cities
} path;

// Init
map *graph(char *file);
void connect(city *src, city *dst, int time);
int hash(char *name, int mod);
path *new_path();

// Search
city *lookup_city(map *map, char *name);
connection *lookup_connection(city *src, city *dst);
int shortest(city *src, city *dst, int left, path *path);
int shortest_path(city *from, city *to, city **path, int k);
int loop(city **path, int k, city *dst);

#endif // _MAP_H_