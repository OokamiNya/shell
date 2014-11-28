/* TODO==========
   redirecting - handle multiple (sort < file.txt > sortedfiles.txt)
   <<< here string; << here doc
   ignore everything after redir symbol
   does tee work

   pipes
   feature: ls ~ (~ for all commands)
   add color to prompt, customize, bold
   sighandler - catch sigint, ctrl d for eof
   command history, up arrow (or ctrl p) for previous command
   directory stack/linked list
   'cd - n' goes to nth previous dir ; dir stack/linklist
   tab completion!!
   method abstraction, utils.c file maybe (for parsing, trim, etc)
   better errors?
     errno's from getcwd
     syntax errors (cmd:";;;;")
     no such file or dir (redirecting)
   final checks to optimize code + memory usage ( valgrind )
     memory leaks?
     perhaps ignore white space rather than trim?
     excessive looping (strlen, other str fxns)
   header file, readme

   NOTES========== 
   trims excess white space and ';'
   does not print all errors (ex: error from getcwd, syntax error)
     ignores multiple ;'s rather than give syntax error (as in bash)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "shell.h"

#define TRUE 1
#define FALSE 0

#define BUFFER_LEN 512
#define HOME getenv("HOME")
#define printError() printf("Error: %s\n", strerror(errno))

int isRedirect = FALSE;
char *redirect_symbol;
char *redirect_file;


void printPrompt(){
  char *cwd = 0;
  cwd = getcwd(cwd,0); // Dynamically allocated
  // If path includes $HOME
  if (strstr(cwd,HOME)){
    int homeLen = strlen(HOME);
    int absPathLen = strlen(cwd);
    int relPathLen = absPathLen-homeLen+1;
    // Replace $HOME with '~'
    strncpy(cwd, &cwd[homeLen-1], relPathLen);
    cwd[0] = '~';
    cwd[relPathLen] = '\0';
  }
  printf("%s$\n", cwd);
  printf("><((((º> ");
  free(cwd);
}

void changeDir(char *arg){
  // If `cd` is given an argument
  if (arg){
    // Replace '~' with $HOME
    if (arg[0]=='~'){
      char *tmp = malloc(strlen(HOME) + strlen(arg));
      strcpy(tmp,HOME);
      strcat(tmp,arg+1);
      strcpy(arg,tmp);
      free(tmp);
    }
    if (chdir(arg) < 0)
      printf("cd: %s: %s\n", arg, strerror(errno));
  }
  else
    chdir(HOME);
}

void redirect(){
  int fd;
  if (!strcmp(redirect_symbol, ">")){
    fd = open(redirect_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (fd < 0)
      printError();
    else{
      if (dup2(fd, STDOUT_FILENO) < 0)
	printError();
      close(fd);
    }
  }
  else if (!strcmp(redirect_symbol,">>")){
  }
  else if (!strcmp(redirect_symbol,"<")){
  }
  else if (!strcmp(redirect_symbol,"<<")){
  }
  else if (!strcmp(redirect_symbol,"|")){
  }
  else{
    printf("This should never happen.. See redirect(argv)\n"); // DEBUGGING
  }
}

void execute(char **argv){
  // Handle "exit" or "quit" command
  if (!strcmp(argv[0], "exit") || !strcmp(argv[0], "quit")){
    printf("Sea ya next time\n");
    free(argv);
    exit(EXIT_SUCCESS);
  }
  // Handle "cd" command
  else if (!strcmp(argv[0], "cd")){
    changeDir(argv[1]);
  }
  // Handle everything else
  else{
    int f, status;
    f = fork();
    if (f < 0)
      printError();
    else if (!f){
      if (isRedirect){
 	redirect();
      }
      // Execute
      if (execvp(argv[0], argv) < 0){
	printf("%s: command not found\n", argv[0]); // Better errors later
	exit(EXIT_FAILURE);
      }
    }
    else{
      wait(&status);
    }
  }
}

// Returns a dynamically allocated char ** argv
char ** parseInput(char *input, char *delim){
  int size, maxSize = 4; // Limit of the number of tokens in argv
  char **argv = malloc(maxSize * sizeof *argv);
  char *arg = strsep(&input, delim);
  char *tmp; // Used to trim whitespace
  // Separate input by `delim` into arg tokens
  for (size = 0; arg; arg = strsep(&input, delim)){
    // Reallocate if out of memory
    if (size == maxSize-2){
      maxSize *= 2;
      argv = realloc(argv, maxSize * sizeof *argv);
    }
    // Trim leading white space and ';' from arg
    while (isspace(*arg) || *arg==';')
      arg++;
    if (*arg){
      // Trim trailing white space and ';'
      tmp = arg + strlen(arg) - 1;
      while (tmp > arg && (isspace(*tmp) || *arg==';'))
	tmp--;
      *(tmp+1) = '\0';
      // Instatiate redirect_file, redirect_symbol, or argv[size]
      if (isRedirect){
	redirect_file = arg;
	break; // Finish after storing the token after the redirect symbol
      }
      else if (!strcmp(arg,">") || !strcmp(arg,">>") || \
	       !strcmp(arg,"<") || !strcmp(arg,"<<") || !strcmp(arg,"|")){
	redirect_symbol = arg;
	isRedirect = TRUE;
      }
      else{
	argv[size] = arg;
	size++;
      }
    }
  }
  argv[size] = NULL;
  return argv;
}

void shell(){
  char *input;
  char **commands = 0;
  char **argv = 0;
  int count;
  while (1){
    // Get user input commands
    printPrompt();
    fgets(input, BUFFER_LEN, stdin);
    // Separate commands (with ;) from input
    commands = parseInput(input, ";");
    // Execute each command
    count = 0;
    while (commands[count]){
      if (isRedirect) isRedirect = FALSE;
      argv = parseInput(commands[count], " ");
      execute(argv);
      free(argv);
      count++;
    }
    free(commands);
  }
}

int main(){
  printf("Welcome to Shellfish!\n");
  shell();
  return 0;
}
