#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MOD 250
#define BUFFER 200
#define DEBUG

/* -------------------------------- Elements -------------------------------- */

typedef struct City City; // Forward declaration

typedef struct Connection {
  City *dst;
  int time;
} Connection;

typedef struct City {
  char *name;
  int id;
  Connection *connections; // Dynamic array of connections
  int size;                // Number of connections
  int capacity;            // Capacity of connections
} City;

typedef struct Map {
  City *cities;            // Dynamic array of cities, Hash table
  int size;                   // Number of cities
  int capacity;            // Capacity of cities
} Map;

typedef struct Path {
  City *city;
  int total_time;
  struct Path *prev;
} Path;

typedef struct PriorityQueue {
  Path **paths;
  int size;
  int capacity;
} PriorityQueue;

/* ---------------------------------- Init ---------------------------------- */

Map *graph(char *file);
City *lookup_city(Map *map, char *name);
void connect(City *src, City *dst, int time);
int hash(char *name, int mod);
void connect(City *src, City *dst, int time);

/* ------------------------- Prior Queue operations ------------------------- */

PriorityQueue *new_priority_queue();
void push(PriorityQueue *pq, Path *p);
Path *pop(PriorityQueue *pq);

/* ------------------------------ Search operations -------------------------- */

Path *dijsktra(Map *map, City *from, City *to);
void print_path(Path *path);
int print_time(Path *path);

#endif // _DIJKSTRA_H_