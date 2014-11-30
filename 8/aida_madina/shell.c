#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int count_commands(char input[256]);
int count_args(char *command);

int main() {
  
  char *command;
  char input[256];
  char *args_array[10];
  char *comm_array[10];
  int num_args = 0;
  int num_commands;
  int pid;
  int *status;
  siginfo_t *infop;
  char cwd[256];
  
  while(1) {
    printf("seashell:%s$ ", getcwd(cwd, sizeof(cwd)));
    fgets(input, sizeof(input), stdin);
    input[strlen(input)-1]='\0';

    num_commands = count_commands(input);
    comm_array[0] = strtok(input, ";");
    int i = 1;
    while (i < num_commands) {
      comm_array[i] = strtok(NULL, ";");
      if(i > num_commands)
        return 0;
      i++;
    }

    for( i = 0; i < num_commands; i++) {
      command = comm_array[i];
      num_args = count_args(command);

      char *comm = strtok(command, " ");
    
      if (!strcmp(comm,"exit")) {
	      exit(0);
      }
    
      args_array[0] = comm;
    
      if (num_args== 0) {
	      args_array[1]=NULL;
      }
    
      else {
	      i = 1;
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
      }
      else {
	      pid = fork();
        if(!pid) {
          execvp(args_array[0], args_array);	
          int exit_val = 105;
          return WEXITSTATUS(exit_val);
        }
	      waitid(P_PID, pid, infop, WEXITED);
      }
    }    
}
}

int count_commands(char input[256]) {
  int num_commands = 1;
  char *commands = input;
  while (*commands) {
    if (*commands == ';'){
      num_commands++;
    }
    commands++;
  }
  return num_commands;
}

int count_args(char *command) {
  char *p = command;
  int num_args = 0;
  while (*p){
    if (*p == ' ') {
      num_args++;
    }
    p++;
  }
  return num_args;
}

