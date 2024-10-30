#include "map.h"

int main() {
  map *trains = graph("./trains.csv");
  path *pathes = new_path();
  int time = shortest(lookup_city(trains, "Malmö"), lookup_city(trains, "Göteborg"), 200, pathes);
  printf("Shortest time: %d\n", time);
  for(int i = pathes->n - 1; i >= 0; i--) {
    printf("%s\n", pathes->cities[i].name);
    if(i > 0)
      printf("%d\n", pathes->times[i]);
  }
  return 0;
}