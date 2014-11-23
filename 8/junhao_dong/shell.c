#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "shell.h"

/*
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
*/

#define BUFFER_LEN 256

/* TODO:
   konami easter eggs??
   fish ascii art on `exit`: http://www.ascii-art.de/ascii/def/fish.txt
   
   REAL TODO:
   change_dir()
   prompt with fish (home dir)
     accept ~ . .. - 
   errno
   method abstraction
   ctrl-d for EOF
 */

char ** parseInput(char *input, char *delim){
  int maxSize = 4; // Limit of the number of tokens in argv
  int size = 0; // Index counter
  char **argv = (char **)malloc(sizeof(char *));
  char *arg = strsep(&input, delim);
  char *tmp; // Used to trim whitespace

  for (; arg; arg = strsep(&input, delim), size++){
    // Reallocate when out of memory
    if (size == maxSize-2){
      maxSize *= 2;
      argv = (char **)realloc(argv, sizeof(char *)*maxSize);
    }
    
    // Trim leading and trailing white space from arg
    while (isspace(*arg))
      arg++;
    if (*arg){
      tmp = arg + strlen(arg) - 1;
      while (tmp > arg && isspace(*tmp))
	tmp--;
    }
    *(tmp+1) = 0;

    // Instantiate the array with tokens separated by `delim`
    argv[size] = arg;
  }

  // Append NULL to follow execvp() syntax
  argv[size] = NULL;

  return argv;
}

void changeDir(){
  
}

// TODO: handle invalid commands
void execute(char **argv){
  // Handle "exit" command
  if (!strcmp(argv[0],"exit")){
    printf("Sea ya next time\n");
    exit(EXIT_SUCCESS);
  }
  // Handle "cd" command
  else if (!strcmp(argv[0],"cd")){
    changeDir();
  }
  else{
    int f, status;
    f = fork();
    // Child executes
    if (!f){
      execvp(argv[0], argv);
      //error stuff
      //having to type exit twice when typing non valid cmd
    }
    else
      wait(&status);
  }
}

void shell(){
  char *input;
  char **argv = (char **)malloc(sizeof(char *));
  int count;

  while (1){
    printf("><((((º> ");
    fgets(input, BUFFER_LEN, stdin);
    
    // Separate commands (with ;) from input
    argv = parseInput(input,";\n");

    // Execute each command
    count = 0;
    while (argv[count] && strcmp(argv[count],"")){
      // DEBUG
      printf("Command: '%s'\n",argv[count]);
      execute(parseInput(argv[count], " "));
      count++;
    }
    
    free(argv);
  }
}

int main(){
  printf("\n=======Welcome to Shellfish, Home of the Selfish=======\n");
  shell();
  return 0;
}
