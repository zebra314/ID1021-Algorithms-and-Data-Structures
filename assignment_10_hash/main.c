#include "hash.h"

int main() {
  codes *postnr = read_postcodes("./postnummer.csv");
  // collisions(postnr, 10000);
  
  test1(postnr);
  return 0;
}