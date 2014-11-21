#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int main() {
  
  char command[256];
  char *args_array[10];
  int num_args = 0;
  int pid;
  int *status;
  siginfo_t *infop;
  char cwd[256];
  
  while(1) {
    printf("seashell:%s$ ", getcwd(cwd, sizeof(cwd)));
    fgets(command, sizeof(command), stdin);
    command[strlen(command)-1]='\0';
    
    char *p = command;

    while (*p){
      if (*p == ' '){
	num_args++;
      }
      p++;
    }

    char *comm = strtok(command, " ");
    //if comm == exit or cd, we take care of their respective processes
    //else, we do what we currently have
    if (!strcmp(comm,"exit")) {
      //printf(":%s:", comm);
      exit(0);
    }
    
    args_array[0] = comm;
    
    if (num_args== 0) {
      args_array[1]=NULL;
    }
    
    else {
      int i = 1;
      while (i <= num_args) {
	args_array[i] = strtok(NULL, " ");
	i++;
      }
      args_array[i]=NULL;
    }

    if (!strcmp(comm,"cd")) {
      if (!args_array[1]) {
	chdir(getenv("HOME"));
      }
      chdir(args_array[1]);
      //execvp(args_array[0], args_array);
    }
    else {
      pid = fork();
      if(!pid) {
	execvp(args_array[0], args_array);	
	return WEXITSTATUS(105);
	exit(0);
      }
      waitid(P_PID, pid, infop, WEXITED);
    }
  }
  
  return 0;
}

