#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#include "executor.h"

//grabs f from executor.c
//f is the pid of kid
extern int f;

static void sighandler(int signo){
  if (signo == SIGINT){
    if (f){
      exit(0);
    }
  }
}

int main(){

  signal(SIGINT, sighandler);

  printf("\n");
  chdir(getenv("HOME"));
  
  while(1){
    printf("pid: %d\n", getpid());

    char input[256];
    char cwd[256];
    getcwd(cwd,256);
    cwd[strlen(cwd)] = 0;

    //replaces home with ~
    char * newcwd = replace_string(cwd,getenv("HOME"),"~");
    
    printf("JAVO:%s> ",newcwd);
    fgets(input, sizeof(input), stdin);
    input[sizeof(input)] = 0;
    
    execute(input); 
  }
  return 0;
}
