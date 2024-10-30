#include "map.h"

int main() {
  map *trains = graph("./trains.csv");
  path *pathes = (path*)malloc(sizeof(path)*100);
  int time = shortest(lookup_city(trains, "Malmö"), lookup_city(trains, "Göteborg"), 200, pathes);
  printf("Shortest time: %d\n", time);
  return 0;
}