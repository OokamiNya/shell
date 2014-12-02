#include "redirect.h"

void redir_in(char *input_file, char* executable) {
	int fd = open(input_file, O_RDONLY, 0644);
	dup2(fd, STDIN_FILENO);
	execvp(executable, NULL);
}

void redir_out(char *output_file, char *exec_args[], char *command, int type){
	int file;
	if (type == 2) {
		file = open(output_file, O_CREAT | O_WRONLY| O_APPEND, 0644);
	}
	else {
		file = open(output_file, O_CREAT | O_WRONLY| O_TRUNC, 0644);
	}
	int pid = fork();
	if (!pid) {
		dup2(file, STDOUT_FILENO);
		execvp(command, exec_args);
	}
}