#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

int count_commands(char input[256]);
int count_args(char *command);

int main() {
  char *command;
  char input[256];
  char *comm_array[10];
  int num_commands;
  int pid;
  int *status;
  siginfo_t *infop;
  char cwd[256];
  int flag_redir = 0;
  FILE * fout = NULL;
  int exit_val = 105;
  int flag_redir_type = 0;

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
      char *args_array[10];
      command = comm_array[i];
      int num_args = count_args(command);
      char *comm = strtok(command, " ");
      if (!strcmp(comm,"exit")) {
	exit(0);
      }
      args_array[0] = comm;
      int j = 1;
      if (num_args == 0) {
	args_array[1]=NULL;
      }
      else {
	while (j <= num_args) {
	  args_array[j] = strtok(NULL, " ");
	  if (!strcmp(args_array[j], ">")) {
	    flag_redir = j;
	  }
	  else if (!strcmp(args_array[j],">>")) {
	    flag_redir_type = 1;
	    flag_redir = j;
	  }
	  j++;
	}
	args_array[j]=NULL;
      }
      if (!strcmp(comm,"cd")) {
	if (!args_array[1]) {
	  chdir(getenv("HOME"));
	}
	chdir(args_array[1]);
      }
      else if (flag_redir) {
	int file;
	if (flag_redir_type) {
	  file = open(args_array[flag_redir + 1], O_CREAT | O_WRONLY| O_APPEND, 0644);
	}
	else {
	  file = open(args_array[flag_redir + 1], O_CREAT | O_WRONLY| O_TRUNC, 0644);
	}
	pid = fork();
	if (!pid) {
	  dup2(file,STDOUT_FILENO);
	  int n;
	  char *exec_args[10];
	  for(n = 0; n < flag_redir; n++) {
	    exec_args[n] = args_array[n];
	  }
	  exec_args[n] = NULL;
	  execvp(args_array[0], exec_args);
	  return 0;
	}
	flag_redir = 0;
      }
      else {
	pid = fork();
	if(!pid) {
	  execvp(args_array[0], args_array);
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
