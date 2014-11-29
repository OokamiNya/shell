#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "util.h"

#define TRUE 1
#define FALSE 0

#define BUFF_SIZE 2048
#define MAX_PIPES 256


//Main splits on newlines and semicolons, then passes each comand string into splitpipes

//this creates pipes for each command,
//then runs doredirects on each, passing in the appropriate fd (stdin/out or a pipe)
int splitpipes(char *command);

//splitpipes gives everything between pipe chars to doredirects
int doredirects(char *command, int infd, int outfd);// in and out can be pipes or stdin/out
//doredirects first handles cd, exit, and help
//Then forks
//Then handles file redirection and argification, then calls docommand
//returns the pid of the child process

//doCommand actually execs the command
//NOTE: here argv[0] is the name of the command to run
int doCommand(char *argv[]); 

int main() {
	char command_buff[BUFF_SIZE];
	char *command = command_buff;
	char *curr_command;
	char exit_flag = FALSE;
	int count, i;
	int file;

	int argc = 0;
	char *argv[MAX_ARGS];

	printf("Starting Interactive mode...\n");
	printf("Type 'help' for help\n");

	while(!exit_flag) {
		print_prompt();

		if((count = _getline(command, BUFF_SIZE)) == -1) { //getline
			printf("Command too long, cannot be more than %d chars\n", BUFF_SIZE);
		} else {
			char *save;
			for(; ; command = NULL) {//Tokenize the string into commands[], split on ;
				if((curr_command = strtok_r(command, ";", &save)) == NULL)
					break;
				splitpipes(curr_command);
			}
			command = command_buff;//Reset command to point to the start of the buffer
		}
	}

	return 0;
}


int splitpipes(char *command) {
	char *commands[MAX_PIPES];
	int pipes[MAX_PIPES][2];//pipes[n] goes from commands[n] to commands[n+1]
	int pids[MAX_PIPES];
	int num_commands, num_children;
	int pid, status;
	int i;

	num_commands = 0;

	char *save;
	for(; ; command = NULL) {//Tokenize the strings into commands[]
		if((commands[num_commands] = strtok_r(command, "|", &save)) == NULL)
			break;
		num_commands++;
	}

	for(i = 0; i < num_commands - 1; i++) {//make n-1 pipes
		if(pipe(pipes[i]) == -1) {
			warn("Failed to open pipe");
			return -1;
		}
	}

	/*while(commands[start]) {
		printf("%s\n", commands[start]);
		start++;
	}*/


	num_children = 0;
	for(i = 0; i < num_commands; i++) {//run the commands
		int in, out;
		if(i == 0)
			in = STDIN_FILENO;
		else
			in = pipes[i - 1][0]; //Read end of pipe

		if(i == num_commands - 1)
			out = STDOUT_FILENO;
		else
			out = pipes[i][1]; //write end of pipe

		pids[i] = doredirects(commands[i], in, out);
		if(pids[i] == -1) {
			fprintf(stderr, "Something went wrong with command %d\n", i);
			num_commands = i - 1;//end looping(if = i-1, wont close last pipe)
		} else if(pids[i] == -2) {
			//cd or help
			//cd doesn't fork off, neither does help, so don't increment num_children
		} else {
			num_children++;
		}

		if(i != 0)//Close the pipe ends used by this command specifically
			close(pipes[i - 1][0]);
		if(i != num_commands - 1)
			close(pipes[i][1]);
	}

	
	for(i = 0; i < num_children; i++) {//wait for each child to finish
		int returned = 0;
		if((returned = wait(&status)) == -1)
			warn("Error with child");
	}

}

int doredirects(char *command, int infd, int outfd) {
	int argc;
	redirect_mode rmode;
	char *argv[MAX_ARGS];
	int readarg, writearg;
	int pid;

	while(*command == ' ')//Eat leading whitespace
		command++;

	if(strstr(command, "help") == command) {
		print_help();
		return -2;
	} else if(strstr(command, "exit") == command) {
		exit(0);
	} else if(strstr(command, "cd") == command) {
		if((argc = argify(command, argv)) == -1) {
			errx(-1, "Too many args");
		}
		return gsh_cd(argv);
	}

	pid = fork();

	if(!pid) { //If child

		dup2(infd, STDIN_FILENO);
		dup2(outfd, STDOUT_FILENO);

		//Need to shift args down sometimes
		//readarg is the next one to be read, writearg is the next one to be written

		//TODO, also cant = MAX_ARGS, need one null element at end for execve
		if((argc = argify(command, argv)) == -1) {
			errx(-1, "Too many args");
		}

		argv[argc] = 0;//argv has to be null-terminated

		//if a redirect is found, redirect args have to be shifted down
		//
		for(readarg = 0, writearg = 0; readarg < argc; readarg++, writearg++) { 
			rmode = 0;
			argv[writearg] = argv[readarg];//shift args down
			//redirects have to be ingored by the command itself
			if(strcmp(argv[readarg], ">") == 0) {
				rmode = REDIRECT_OUT_FLAG;
			} else if(strcmp(argv[readarg], "2>") == 0) {
				rmode = REDIRECT_ERR_FLAG;
			} else if(strcmp(argv[readarg], "&>") == 0) {
				rmode = REDIRECT_ERR_FLAG
					| REDIRECT_OUT_FLAG;
			} else if(strcmp(argv[readarg], ">>") == 0) {
				rmode =   REDIRECT_OUT_FLAG 
					| REDIRECT_APPEND_FLAG;
			} else if(strcmp(argv[readarg], "2>>") == 0) {
				rmode = REDIRECT_ERR_FLAG 
					| REDIRECT_APPEND_FLAG;
			} else if(strcmp(argv[readarg], "&>>") == 0) {
				rmode = REDIRECT_ERR_FLAG 
					| REDIRECT_OUT_FLAG
					| REDIRECT_APPEND_FLAG;
			} else if(strcmp(argv[readarg], "<") == 0) {
				rmode = REDIRECT_IN_FLAG;
			}
			if(rmode != 0) {

				if(readarg + 1 == argc) {//If theres no file to redirect to
					warnx("Error: expected filename after '>'\n");
					writearg--; //Eats up the '>', proceeds normally
				} else {
					readarg++; //Moves up to the filename
					if(redirect(argv[readarg], rmode) == -1) {
						warnx("Error: redirect failed");
						return -1;
					}

					//Writearg should now be '>'
					writearg--;
					//next loop, writearg will also be '>' and will
					//overwrite it with whatever is past the filename

				}
			}
		}
		argv[writearg] = argv[readarg];

		doCommand(argv);

		return -1;//THIS SHOULDNT HAPPEN
	} else {
		return pid;
	}
}

int doCommand(char *argv[]) {//Already forked
	execvp(argv[0], argv);
	perror("execve");

}

