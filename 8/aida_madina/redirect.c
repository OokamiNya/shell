#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

int redir_in(char **args) {
	int input_file = open(args[2], O_RDONLY, 0644);
	dup2(input_file, STDIN_FILENO);
    execvp(args[0], NULL);
}

int redir_out(char **args, int index) {
	int file;
	if (!strcmp(args[index],"<")) {
		file = open(args[index + 1], O_CREAT | O_WRONLY| O_TRUNC, 0644);
	}
	else {
		file = open(args[index + 1], O_CREAT | O_WRONLY| O_APPEND, 0644);
	}
	int pid = fork();
	if (!pid) {
	  dup2(file,STDOUT_FILENO);
	  int n;
	  char *exec_args[10];
	  for(n = 0; n < index; n++) {
	    exec_args[n] = args[n];
	  }
	  exec_args[n] = NULL;
	  execvp(args[0], exec_args);
	  return 0;
	}
	return 0;
}