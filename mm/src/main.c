#include <stdio.h>
#include "my_lib.h"

int main(int argc, char * argv[]) {
  if(argc < 2) {
    printf("Error: Name not provided.\n");
    return -1;
  }

  hello(argv[1]);
}
