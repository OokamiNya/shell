#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "signal.h"

int main(){

  chdir(getenv("HOME"));
  
  signal(SIGINT, sighandler);
  while(1){

    char input[256];
    char cwd[256];
    getcwd(cwd,256);
    printf("JAVO:>%s ",cwd);
    fgets(input, sizeof(input), stdin);
    input[sizeof(input)] = 0;
    printf("input: %s\n", input);

    if(!strcmp(input,"exit")){
      exit(-1);
    }
    else if(strncmp(input,"cd",2)){
      execute(input);
    }
    else{
      int f = fork();
      wait();
      if (!f){
	execute(input);
      }
    }
  }
  
  return 1;

}
