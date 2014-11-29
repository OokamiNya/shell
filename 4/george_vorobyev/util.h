#ifndef UTIL_H
#define UTIL_H

#define STDIN_FD 0
#define MAX_ARGS 256

//These are &ed together to become a redirect_mode
typedef int redirect_mode;
#define REDIRECT_OUT_FLAG 	1
#define REDIRECT_IN_FLAG 	2
#define REDIRECT_ERR_FLAG 	4
#define REDIRECT_APPEND_FLAG 	8

//takes an input string (like 'ls -l')
//puts each param into argv
//returns argc, or -1 if error
int argify(char *command, char *argv[]);

//takes in one line of input, up to bytestoread long
//discards up till \n if too long
int _getline(char *in, int bytestoread);

//does a particular redirect, including opening the file and 'dup2'ing
int redirect(char *filename, redirect_mode mode);

//does cd, returns -2 on success, -1 on error
int gsh_cd(char *argv[]); //has to be NULL terminated

//prints "$CWD>" and flushes output
void print_prompt();

//prints help message
void print_help();

#endif
