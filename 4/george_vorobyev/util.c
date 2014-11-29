#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "util.h"

#define PROMPT_SIZE 512

static char prompt[PROMPT_SIZE];

int argify(char *command, char *argv[]) {
	int i = 0;
	while(*command == ' ')//Eat leading whitespace
		command++;


	while(*command) {
		if(i == MAX_ARGS - 1) {
			printf("Error, can't have more than %d args\n", MAX_ARGS);
			return -1;
		}
		argv[i++] = command;
		while(*command != ' ' && *command)//Eat arg
			command++;
		
		while(*command == ' ') {
			*command = '\0';
			command++;
		}
	}
	return i;
}

int _getline(char *in, int bytestoread) {
	int count;
	count = read(STDIN_FD, in, bytestoread);
	
	if(count == -1){
		printf("%d\n", STDIN_FD);
		err(-1, "_getline failed");
	}
	else if(count == 0) 
		errx(-1, "stdin read 0");
	else if(in[count - 1] != '\n') {
		do {
			count = read(STDIN_FD, in, bytestoread);
			if(count == -1)
				errx(-1, "Error after buffer overflow");	
		} while (in[count - 1] != '\n');
		return -1;
	}

	in[count - 1] = '\0';
	return 0;
}

int redirect(char *filename, redirect_mode mode) {
	int fd;
	int flags = 0;

	if(
		(mode & REDIRECT_IN_FLAG) &&
		(mode & (REDIRECT_OUT_FLAG | REDIRECT_ERR_FLAG))
		) 
	{
		flags = O_RDWR | O_CREAT;
	} else if(mode & REDIRECT_IN_FLAG) {
		flags = O_RDONLY;
	} else if (mode & (REDIRECT_OUT_FLAG | REDIRECT_ERR_FLAG)) {
		flags = O_WRONLY | O_CREAT;
	} else {
		warnx("No flags in redirect, shouldn't be happening");
		return -1;
	}
	
	if(mode & REDIRECT_APPEND_FLAG)
		flags |= O_APPEND;
	else if (mode & (REDIRECT_OUT_FLAG | REDIRECT_ERR_FLAG))
		flags |= O_TRUNC;
	
	
	if((fd = open(filename, flags, 0666)) == -1) {
		warn("Error, failed to open file for redirection");
		return -1;
	}


	if(mode & REDIRECT_IN_FLAG)
		if(dup2(fd, STDIN_FILENO) == -1) {
			warn("Error, failed to redirect");
			return -1;
		}
	if(mode & REDIRECT_OUT_FLAG)
		if(dup2(fd, STDOUT_FILENO) == -1) {
			warn("Error, failed to redirect");
			return -1;
		}
	if(mode & REDIRECT_ERR_FLAG)
		if(dup2(fd, STDERR_FILENO) == -1) {
			warn("Error, failed to redirect");
			return -1;
		}

	return 0;
}

//returns -2 on success, -1 on error
int gsh_cd(char *argv[]) {
	int argc = 0;
	while(argv[argc])
		argc++;
	
	if(argc < 2 || argc > 2) {
		printf("Usage: cd [PATH]\n");
		return -2;
	}

	if(chdir(argv[1]) == -1) {
		warn("Error in cd");
		return -1;
	}

	return -2;
}

void print_prompt() {
	if(getcwd(prompt, sizeof(prompt)) == NULL) {
		errx(-1, "Path too long\n");
	}

	printf("%s>", prompt);
	fflush(stdout);
}

void print_help() {
	printf("Type 'help' for help\n");
	printf("Type 'cd' to change directory\n");
	printf("Any other commands will be run from /bin/\n");

	printf("This shell handles >, 2>, &>, >>, 2>>, &>>,");
	printf(" and <, as well as pipes and semicolons\n");
}

