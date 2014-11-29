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
    int spoon = fork();
    int status;
    if (!spoon) {
      execvp(input[0],input);
    }
    else {
      wait(&status);
    }
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
  //somehow this works
  //printf("%d\n", i+2);
  char** argv = calloc(i+2,sizeof(char *));
  argv[0] = arg;
  int j = 0;
  while(arg){
    j++;
    arg = strtok(NULL, delim);
    argv[j] = arg;
  }

  int k = 0;
  for (;k<i+2; k++) {
    printf("argv[%d]: %s\n", k, argv[k]);
  }

  return argv;
}

int main() {
  char cwd[256];
  int running = 1;

  char input[256];
  //char** commands = 0;
  //char** args = 0;
  //char** argv = 0;

  while (running) {
    char** args = 0;
    char** argv = 0;

    printf("(╯'□')╯%s: ", getcwd(cwd, sizeof(cwd)));
    
    fgets(input, sizeof(input), stdin);
    
    //printf("Unparsed:\n");
    args = parse(input,";"); //args has a bunch of commands unparsed by space, parsed by ;
    //printf("Parsed:\n");

    int i = 0;
    while (args[i]) { //execute every command
      argv = parse(args[i]," ");
      execute(argv);
      free(argv);
      i++;
    }

    //argv = parse(input," ");
    //execute(argv);
    //free(argv);
  }

  return 0;
}
