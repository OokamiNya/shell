/**
 * Shell.c
 * Contains main()
 *
 **/

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
  printf("done\n");
  return 0;
  
  
}

int main() {
  int running = 1;
  char input[256];
  while (running) {
    printf("(╯'□')╯︵ ┻━┻ ");
    fgets(input, sizeof(input), stdin);
    int spoon = fork();
    if (spoon < 0) {
      printf("fork failed!\n");
    }
    else if (!spoon) {
      execute(input);
      exit(0);
    }
    printf("ended\n");
  }
  return 0;
}
