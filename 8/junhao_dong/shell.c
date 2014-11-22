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
#define ARGV_LEN sizeof(char *) * 16

/* TODO:
   konami easter eggs??
   fish ascii art on `exit`: http://www.ascii-art.de/ascii/def/fish.txt
   
   REAL TODO:
   whitespace
   write cd + print dir with fish: pwd, dup2?; check piazza
     accept ~ . .. - 
   memory usage; malloc those arrays more carefully
   ERRNOs
   // free argv after done with it

   NOTES/ISSUES:
   1) 
 */

char ** parseInput(char *input, char *delim){
  int count = 0;
  char **argv = (char **)malloc(ARGV_LEN);
  char *arg = strsep(&input,delim);
  

  for (; arg; arg = strsep(&input, delim), count++){
    argv[count] = arg;
  }
  argv[count] = NULL;
  return argv;
}

void changeDir(){
  
}

void execute(char **argv){
  if (!strcmp(argv[0],"exit")){
    printf("Sea ya next time\n");
    exit(EXIT_SUCCESS);
  }
  else if (!strcmp(argv[0],"cd")){
    changeDir();
  }
  else{
    int f, status;
    f = fork();
    if (!f){ //child
      execvp(argv[0], argv);
      //error stuff
    }
    else
      wait(&status);
  }
}

void shell(){
  char *input;
  char **argv = (char **)malloc(ARGV_LEN);
  while (1){
    printf("><((((º> ");
    fgets(input, BUFFER_LEN, stdin);
    
    argv = parseInput(input,";\n");
    while (*argv && strcmp(*argv,"")){ // empty string at end
      printf("-%s-\n",*argv);
      execute(parseInput(*argv," "));
      *argv++;
    }
  }
}

  //  int count, arrLen = 1;//sizeof(argv)/sizeof(char *);

int main(){
  printf("\n=======Welcome to Shellfish, Home of the Selfish=======\n");
  shell();
  return 0;
}
