#include "hash.h"

int main() {
  codes *postnr = read_postcodes("./postnummer.csv");
  
  // test1(postnr);
  // test2(postnr);
  // test3(postnr);
  // test4(postnr);
  test5(postnr);
  return 0;
}