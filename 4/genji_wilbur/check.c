#include "check.h"

int check_cd(char * command)
{
	//location of cd substring in command
	char * cdloc;
	cdloc = strstr(command, "cd");
	if (cdloc == command){
		return 1;
	}
	return 0;
}

//similar in nature to check_cd
int check_exit(char * command)
{
	char * exitloc;
	exitloc = strstr(command, "exit");
	if (exitloc == command){
		return 1;
	}
	return 0;
}

//checks for special characters and executes using them
int check_special(char * command)
{
	/*
	if (check_semicolon(command)){
		return run_multiple_commands(command);
	} else if (check_redirection(command)){

	}
	*/
	return 0;	
}

