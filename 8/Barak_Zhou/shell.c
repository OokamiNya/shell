#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute(char* input) {
  
  char** args = NULL;
  char* ptr = strtok(input, " ");
  int spaces = 0;
  while (ptr) {
    args = realloc( args, sizeof(char*) * ++spaces );
    args[spaces-1] = ptr;
    ptr = strtok(NULL, " ");
  }
  strtok(args[spaces-1], "\n");

  args = realloc ( args, sizeof(char*) * (spaces+1) );
  args[spaces] = 0;

  execvp(args[0],args);
  return 0;
}


int main() {
  int running = 1;
  char input[256];
  while (running) {
    printf("$tacks on $tacks: ");
    fgets(input,256,stdin);
    //printf("%s", input);
    execute(input);

  }
  
  return 0;
}
