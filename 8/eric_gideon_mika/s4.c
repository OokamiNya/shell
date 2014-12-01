/*
 * todo:
 * handle extra spaces for exec ("ls " breaks)
 * extend execute so it doesn't rely on space separation
 * print specific errors when they occur
 */

/* Handles All Parsing Functions. Also contains int main() */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

#include "executor.h"

/* int count_tokens() (Idea from dwshell in DESIGN_EXAMPLE.txt)
Inputs: char *s - string to be parsed
        char delim - character to separate on
Returns: Number of tokens separated by delim + 1

Counts number of times the character delim appears in the string line
Number of tokens is 1 more than appearences of delim
if delim is not in the string, returns 1 */
int count_tokens(char *s, char delim){
	int c = 1;
	char *p = s-1;
	while ((p = strchr(p+1, delim)))
		c++;
	return c;
}

/* char **split(char *s, char delim)
Inputs: char *s - string to be parsed
        char delim - character to separate on
Returns: Array of Tokens separated by delim

splits the string into tokens, separated by delim
max size is 1000 * sizeof(char *) */
char **split(char *s, char delim){
	char d[2] = {delim, '\0'};
	char **sep = malloc(sizeof(char*) * 1000);
	int i = 0;
	while((sep[i] = strsep(&s, d)))
		i++;
	return sep;
}


/* static void handle_argument()
Inputs: int argument_number - the argument the parser is on, each argument is separated by a | for our case
        char **lpipe - array of all the tokens, separated by |
        int npipes - the total number of tokens, separated by |
Returns: No return value

handles the input/output of each token
    sends the output of one command to a file, which the next command will use as input and so on (.f1, .f2 are the two files)
reads lpipe[argument_number] and executes the token contained in that string using executor.c
waits for child process to finish */
static void handle_argument(int argument_number, char **lpipe, int npipes) {
        int fdin, fdout;
        /*
         * alternate between in and out files so that the next function reads
         * from the previous function's out file and writes output to the in file
         */
        if(argument_number%2 == 0){
                fdin = open(".f1", O_RDONLY);
                fdout = open(".f2", O_CREAT | O_WRONLY | O_TRUNC, 0777);
        }
        else {
                fdin = open(".f2", O_RDONLY);
                fdout = open(".f1", O_CREAT | O_WRONLY | O_TRUNC, 0777);
        }

        // if it's the first argument, read from stdin
        if(argument_number == 0)
                fdin = 0;

        // if it's the last argument, print the output to the shell
        if(argument_number == npipes)
                fdout = 1;

        // execute the command
        int childpid = execute(lpipe[argument_number], fdin, fdout);

        // don't close stdin or stdout
        if (argument_number != 0)
            close(fdin);
        if (argument_number != npipes) {
            close(fdout);
        }

        // wait for child process before continuing if it's the last command
        if (argument_number == npipes && childpid != -1) {
                int returnStatus;
                waitpid(childpid, &returnStatus, 0);
        }
}

/* static int handle_pipes()
Inputs: char **command_chain - array of all commands, separated by semicolon ';'
Returns: No return value

handles the 'exit' case, in which case it ends all processes and exits gemshell
splits on pipes "|" and enters a while loop, calling handle_argument for each token separated by pipe */
static void handle_pipes(char **command_chain) {
        int i, arg_number, npipes;
        char **lpipe;
        while (command_chain[i]) {
                // "exit" case
                if (!strcmp("exit", command_chain[i])) {
                    exit(0);
                }
                // "cd" case is handled in executor because we must split on spaces first
                npipes = count_tokens(command_chain[i], '|') - 1;
                lpipe = split(command_chain[i], '|');
                arg_number = 0;
                while (lpipe[arg_number]) {
                        handle_argument(arg_number, lpipe, npipes);
                        arg_number++;
                }
                i++;
        }
}


/* static void run()
Inputs: no Inputs
Returns: No Return value

uses fgets to read user input
splits on semicolons and pass value to handle_argument() */
static void run() {
	while(1){
                char buf[1000], **l;
		printf("gemshell> ");
		fgets(buf, sizeof buf, stdin);
		*strchr(buf, '\n') = 0;

		l = split(buf, ';');
                handle_pipes(l);
	}
}

int main(){
        run();
        return 0;
}
