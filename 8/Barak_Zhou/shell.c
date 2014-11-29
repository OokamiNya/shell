/**
 * Shell.c
 * Contains main()
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int execute(char** input) {
  if (!strcmp(input[0], "cd")){
    if (!input[1]) {
      chdir(getenv("HOME"));
    }
    else if (chdir(input[1]) < 0) {
      printf("cd: %s: %s\n", input[1], strerror(errno));
    }
  }
  else if (!strcmp(input[0], "exit")){
    exit(EXIT_SUCCESS);
  }
  else {
    // getting there
  }
  return 0;
}

char** parse ( char* input, char* delim ) {
  strtok(input, "\n"); //this guy again...

  int i = 0;
  char* str = input;
  while(*str){
    if(*str == *delim){
      i++;
    }
    str++;
  }
  char* arg = strtok(input, delim);
  char** argv = (char **)malloc(sizeof(char *)* i+1);
  argv[0] = arg;
  int j = 0;
  while(arg){
    j++;
    arg = strtok(NULL, delim);
    argv[j] = arg;
  }
  //printf("arg[1] %s\n", argv[1]);
  return argv;
}

int main() {
  char cwd[256];
  int running = 1;

  char input[256];
  //char** commands = 0;
  char** argv = 0;

  while (running) {
    printf("(╯'□')╯%s: ", getcwd(cwd, sizeof(cwd)));
    
    fgets(input, sizeof(input), stdin);
    argv = parse(input," ");
    execute(argv);
    free(argv);
  }

  return 0;
}
