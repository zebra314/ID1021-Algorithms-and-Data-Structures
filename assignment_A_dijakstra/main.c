#include "dijkstra.h"

int main() {
  Map *map = graph("europe.csv");
  City *from = lookup_city(map, "Malmö");
  City *to = lookup_city(map, "Kiruna");

  Path *p = dijsktra(map, from, to);

  if(p == NULL) {
    printf("No path found\n");
  } else {
    print_path(p);
    printf("Total time: %d\n", print_time(p));
  }

  return 0;
}