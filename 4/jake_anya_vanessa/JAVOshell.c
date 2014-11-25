#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

static void sighandler(int signo){
  if (signo == SIGINT){
    printf("we stopped a thing?\n");
  }
}

int main(){

  printf("\n");
  chdir(getenv("HOME"));
  signal(SIGINT, sighandler);
  
  while(1){

    char input[256];
    char cwd[256];
    getcwd(cwd,256);
    cwd[strlen(cwd)] = 0;
    //substituting home with ~
    /* char * newcwd; */
    /* if(!strncmp(cwd,getenv("HOME"),strlen(getenv("HOME")))){ */
    /*   newcwd = "~"; */
    /*   strncat(newcwd, strstr(cwd,getenv("HOME")), sizeof(cwd)-sizeof(getenv("HOME"))); */
    /* } */
    /* else */
    /*   newcwd = cwd; */
    //
    
    printf("JAVO:%s> ",newcwd);
    fgets(input, sizeof(input), stdin);
    input[sizeof(input)] = 0;

    if(!strcmp(input,"exit\n")){
      exit(-1);
    }
    else if(!strncmp(input,"cd",2)){
      execute(input);
    }
    else{
      int f = fork();
      wait();
      if (!f){
	signal(SIGINT, sighandler);
	execute(input);
      }
    }
  }
  
  return 1;

}
