#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>


char* trim(char *);
/*======== char * trim() =======================
  Inputs:  char *s
  Returns: Pointer to the beginning of a string.
  
  Removes leading and trailing whitespace on the string s. Terminating '\0' is placed at the appropriate new location by strndup.
  ==============================================*/

void printprompt();
/*======== void printprompt() =======================
  Inputs:  N/A
  Returns: N/A
  
  Prints (to stdout) our shell's prompt, which is meant to mimic Linux's bash prompt. It is the concatenation of the user's username, :, and Linux-esque pwd (~/stuff/substuff).
  For example...
  if username is 'unicorns', pwd is '/home/unicorns/Desktop', our prompt will print 'unicorns:~/Desktop'	
  ==============================================*/ 

void execute();
/*======== void execute() =======================
  Inputs:  char a[256]
  Returns: N/A
  
  Executes a single command (ls -l | wc would be considered one command in our case). First, we check for any type of redirection-esque commands (<, >, >>, |) and take care of those commands. If there are no redirection-esque commands, then we simply parse on spaces. If our command is 'cd' or 'exit' or 'quit' then we cd, exit, or quit. Otherwise, we fork and let our child process execvp and run the command. 
  ==============================================*/

void cd(char *);
/*================= void cd() =======================
  Inputs:  char* path
  Returns: N/A
  
  Changes our current working directory in a Linux-esque fashion:
  If not given any arguments, then change working directory to ~. Otherwise, it will change the current working directory to the given argument or yell at you.
  ==============================================*/ 

char** parser();
/*======== char** parser() =======================
  Inputs:  N/A
  Returns: An array of strings, in which each element is a command to be executed.
  
  Takes the user input and parses along the semicolons (;), adding each element into the args array of strings. Then, we return this array of strings.	
  ==============================================*/ 

char* get_id();
/*======== char* get_id() =======================
  Inputs:  N/A
  Returns: A string representing the currently-logged-in user's uid.
  
  A simple helper function which returns the current user's uid as a dynamically allocated string.
  ==============================================*/ 
