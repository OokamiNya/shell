#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#include "executor.h"

int cont = 1;

//grabs f from executor.c
//f is the pid of kid, 0 if no kid
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
  
  while(cont){
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
    
    char *s1 = input;
    char *s2;
    char *arg[256];
    
    int i = 0;
    while(s1) {
      //generates arg (array of arguments)
      s2 = strsep(&s1, " ");
      arg[i] = s2;
      i++;
    }
    
    /*
      minor cleaning up
      last argument = NULL for execvp to work
      newstr is the new second-to-last argument with \n removed
    */
    arg[i] = 0;
    char * newstr;
    newstr = strsep(&(arg[i-1]), "\n");
    arg[i-1] = newstr;
    execute(arg); 
  }
  return 0;
}
