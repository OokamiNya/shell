#include <stdio.h>
#include <stdlib.h>

int main() {
  char * input = malloc(1024);
  fgets(input, 1024, stdin);
  printf("You said: %s\n", input);
  return 0;
}
