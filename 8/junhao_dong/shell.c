#include "shell.h"

#define TRUE 1
#define FALSE 0
#define BUFFER_LEN 256

/* TODO:
   `exit` + konami easter eggs??
   fish ascii art on `exit`: http://www.ascii-art.de/ascii/def/fish.txt
   optimize memory usage: malloc arrays with variable size
   
   REAL TODO:
   write cd + print dir with fish: pwd, dup2?
     accept ~ . .. - 
   handle ; parsing

   NOTES/ISSUES:
   1)
   
 */

char ** parseInput(char *input, char *tok){
  int argc = 0;
  char **argv = (char **)malloc(BUFFER_LEN);
  char *arg;

  for (arg = strsep(&input,tok); *arg; arg = strsep(&input, tok), argc++){
      argv[argc] = arg;
  }
  argv[argc] = NULL;
  return argv;
}

void changeDir(){
  
}

void execute(char **argv){
  //"exit" command
  if (!strcmp(argv[0],"exit")){
    execlp("echo","echo","Sea ya next time",NULL);
  }
  //"cd" command
  else if (!strcmp(argv[0],"cd")){
    changeDir();
  }
  else{
    int f, status;
    f = fork();
    if (!f)
      execvp(argv[0], argv);
    else
      wait(&status);
  }
}

void shell(){
  char *input;
  while (TRUE){
    printf("><((((º> ");
    fgets(input, BUFFER_LEN, stdin);
    // finish later; for (int i = 0; i < parseInput(input,";")
    execute(parseInput(input," \n"));
  }
}

int main(){
  printf("\n=======Welcome to Shellfish, Home of the Selfish=======\n");
  shell();
  return 0;
}
