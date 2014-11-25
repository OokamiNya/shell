#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
  char * input = malloc(1024);
  fgets(input, 1024, stdin);
  input[strlen(input)-1] = 0;
  //printf("You said: %s\n", input);
  printf("%d\n", execlp(input, input, NULL));
  printf("%s\n", strerror(errno));
  return 0;
}
