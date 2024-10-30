#include "map.h"

struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
  return (t_stop->tv_nsec - t_start->tv_nsec) +
         (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

void free_connections(connection *connections, int n) {
    for (int i = 0; i < n; i++) {
        free(connections[i].dst);  // Free each destination city pointer in connections
    }
    free(connections);  // Free the connection array
}

void free_cities(city *cities, int n) {
    for (int i = 0; i < n; i++) {
        // free(cities[i].name);  // Free the name of the city
        // free_connections(cities[i].connections, cities[i].n);  // Free each city's connections
        // free(cities[i].recent_connections);  // Free recent connections if dynamically allocated
    }
    free(cities);  // Free the city array
}

void free_map(map *trains) {
    free_cities(trains->cities, trains->n);  // Free cities in the map
    free(trains);  // Free the map itself
}

void free_path(path *pathes) {
    free(pathes->cities);  // Free cities array in path
    free(pathes->times);   // Free times array in path
    free(pathes);          // Free path structure itself
}


int main() {
  map *trains = graph("./trains.csv");
  path *pathes = new_path();

  clock_gettime(CLOCK_REALTIME, &t_start);

// int time = shortest(lookup_city(trains, "Malmö"), lookup_city(trains, "Göteborg"), 197, pathes);
// int time = shortest(lookup_city(trains, "Göteborg"), lookup_city(trains, "Stockholm"), 400, pathes);
// int time = shortest(lookup_city(trains, "Malmö"), lookup_city(trains, "Stockholm"), 500, pathes);
// int time = shortest(lookup_city(trains, "Stockholm"), lookup_city(trains, "Sundsvall"), 800, pathes);
// int time = shortest(lookup_city(trains, "Stockholm"), lookup_city(trains, "Umeå"), 1100, pathes);
// int time = shortest(lookup_city(trains, "Göteborg"), lookup_city(trains, "Sundsvall"), 1000, pathes);
// int time = shortest(lookup_city(trains, "Sundsvall"), lookup_city(trains, "Umeå"), 500, pathes);
// int time = shortest(lookup_city(trains, "Umeå"), lookup_city(trains, "Göteborg"), 1400, pathes);
int time = shortest(lookup_city(trains, "Göteborg"), lookup_city(trains, "Umeå"), 1400, pathes);

  clock_gettime(CLOCK_REALTIME, &t_stop);

  printf("Shortest time: %d\n", time);
  printf("Compute time: %ld ns\n", nano_seconds(&t_start, &t_stop));
  
  // for(int i = pathes->n - 1; i >= 0; i--) {
  //   printf("%s\n", pathes->cities[i].name);
  //   if(i > 0)
  //     printf("%d\n", pathes->times[i]);
  // }

  free_map(trains);
  free_path(pathes);

  return 0;
}