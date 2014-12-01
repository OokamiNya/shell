#include "run.h"
#include "redir.h"
#include "check.h"
#include "strip_spaces.h"

/*
 * Runs a command.
 * Expects chain and redirection operators to be filtered out prior.
 * Functions currently supported:
 * 	* All exec-able functions
 * 	* cd
 * 	* exit
 * Supports redirection: if redirection not wanted, put NULL for filename
 */
int run( char * cmd, char * filename, int fd[], int oflag )
{
	int status;

	cmd = strip_spaces( cmd );

	if ( check_cd( cmd ) ){
		status = run_cd( cmd, filename, fd, oflag );
	} else if ( check_exit( cmd ) ){
		status = run_exit( cmd, filename, fd, oflag );
	} else {
		status = run_exec( cmd, filename, fd, oflag );
	}
	
	return status;
}

/*
 * Parses a command in preparation for an execvp call.
 * args is a pointer to a string array.
 */
void parse_exec( char * cmd, char *** args )
{
	char * cmdname;
	
	//gets the name of the command and sets it to the first position of the array
	cmdname = strsep(&cmd," ");
	(*args)[0] = cmdname;

	int index = 1;
	//gets arguments
	while (cmd){
		(*args)[index] = strsep(&cmd," ");
		index++;
	}

	//array must null-terminate
	(*args)[index] = NULL;
}

/*
 * Runs a function using execvp.
 */
int run_exec(char * cmd, char * filename, int fd[], int oflag)
{
	char ** args;
	args = (char **)malloc(256 * sizeof(char *));

	parse_exec(cmd, &args);

	int f,w,status;
	f = fork();
	if (f){//parent waits
		w = wait(&status);
	} else {//child executes
		
		if (filename) {
			int fd1 = open( filename, oflag, 0644);
			
			if( errno ){
				if( errno != 22){
					printf("-znshell: %s (%d)\n",strerror(errno),errno);
				}
			}
			
			int x;
			for(x=0;x<(sizeof(fd)/sizeof(fd[0]));x++) {
				if( fd[x]>=0 ){// -1 is null
					dup2(fd1, fd[x]);
				}
			}
		}

		execvp(args[0],args);
		if (errno){
			fprintf(stderr,"-znshell: %s\n",strerror(errno));
			errno = 0;
		}
		exit(0);
	}

	free( args );
	
	return status;
}

//changes directory into the directory specified by the argument
int run_cd(char * command, char * filename, int fd[], int oflag)
{
	
	if (filename) {
		int fd1 = open( filename, oflag, 0644);
		if( errno ){
			if (errno != 22) {
				printf("-znshell: %s (%d)\n",strerror(errno),errno);
			}
		}
		int x;
		for(x=0;x<(sizeof(fd)/sizeof(fd[0]));x++) {
			if(fd[x]>=0) {
				dup2(fd1, fd[x]);
			}
		}
	}

	char * temp;
	temp = strsep(&command," ");
	//command now has the directory path

	int status;
	status = chdir(command);

	//status is 0 if it works, -1 if not

	if(status == 0){
		//printf("Directory changed!\n");
	} else {
		printf("Directory %s not found.\n",command);
	}
	return status;
}

int run_exit(char * command, char * filename, int fd[], int oflag){
	if (filename) {
		int fd1 = open( filename, oflag, 0644);
		if( errno ){
			if (errno != 22) {
				printf("-znshell: %s (%d)\n",strerror(errno),errno);
			}
		}
		int x;
		for(x=0;x<(sizeof(fd)/sizeof(fd[0]));x++) {
			if(fd[x]>=0){ //set null to be -1
				dup2(fd1, fd[x]);
			}
		}
	}
	exit(0);
}


