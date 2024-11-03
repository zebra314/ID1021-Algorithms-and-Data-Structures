#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

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


#endif // _DIJKSTRA_H_