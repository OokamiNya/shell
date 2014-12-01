#include "prompt.h"
#include "parser.h"
#include "redir.h"
#include "strip_spaces.h"

//this will contain all of the parsing functions

//main parsing function
//input will have a newline character from the fgets
int parse_input(char * input){
	//takes out the newline character
	input = strsep(&input,"\n");
	
	//printf("input after stripping: %s\n",input);

	//Semicolon Chaining
	char * cmd;
	int status;
	do {
		cmd = strsep( &input, ";" );
		cmd = strip_spaces(cmd);
		status = process_redir( cmd );
	} while ( input );

	return 0;
}
/*
//run multiple commands with the ;
int run_multiple_commands(char * command){
	char * temp;
	while(command){
		temp = strsep(&command,";");
		temp = strip_spaces(temp);
		execute_command(temp);
	}

	return 1;
}

//for > and < currently
int check_redirection(char * command){
	check_redirect_output(command);
	check_redirect_input(command);
	return 0;
}

//for >, runs redirection procedure if true
int check_redirect_output(char * command){
	if (strchr(command,'>')){
		//stores the actual command
		char * cmd;

		//command now has the file path
		cmd = strsep(&command,">");
		cmd = strip_spaces(cmd);
		command = strip_spaces(command);
		//temporary stdout file descriptor
		int stdoutfd;
		stdoutfd = dup(STDOUT_FILENO);
		//target file descriptor
		int targetfd;

		return 1;
	}
	return 0;
}

//for <, runs redirection procedure if true
int check_redirect_input(char * command){
	if (strchr(command,'<')){
		return 2;
	}
	return 0;
}
*/
