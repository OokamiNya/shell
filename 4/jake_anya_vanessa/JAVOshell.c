#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

static void child_sighandler(int signo){
  if (signo == SIGINT){
    kill(getpid(), SIGKILL);
  }
}

static void parent_sighandler(int signo){
  printf("buttsbuttsbuttsbutts\n");
}

int main(){


  printf("\n");
  chdir(getenv("HOME"));
  //  signal(SIGINT, parent_sighandler);
  
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
      if (!f){ //child process stuff
	//	signal(SIGINT, child_sighandler);
	execute(input);
      }
    }
  }
  
  return 1;

}
