#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "shell.h"

#define BUFFER_LEN 256
#define HOME getenv("HOME")

/* TODO:
   konami easter eggs??
   fish ascii art on `exit`: http://www.ascii-art.de/ascii/def/fish.txt
   
   REAL TODO:
   bug: multiple spaces between command name and arguments are not ignored
   print prompt with ~
   'cd' accepts ~ as part of the path
   pipes and redirecting
   sighandler - catch sigint, ctrl d for eof
   method abstraction, utils.c file maybe (for parsing, trim, etc)
   better errors
   add color to prompt
   command history, up arrow for previous command
   'cd -' goes to previous directory
   tab completion
   design.txt thing

   NOTES/FEATURES: (will be moved to README later)
   does not handle all errors (ex: error from getcwd)
   ignores multiple ;'s rather than give syntax error (as in bash)
 */


void printPrompt(){
  char *cwd = getcwd(cwd,0);
  // If path includes $HOME
  if (strstr(cwd,HOME)){
    int absPathLen = strlen(cwd);
    int homeLen = strlen(HOME);
    // Replace $HOME with '~'
    strncpy(cwd, &cwd[homeLen] - 1, absPathLen - homeLen + 1);
    *cwd = '~';
  }
  printf("%s$\n", cwd);
  printf("><((((º> ");

  free(cwd);
}

void changeDir(char *arg){
  // If `cd` is given an argument
  if (arg){
    // Replace '~' with $HOME
    if (arg[0]=='~')
      arg = strncat(HOME,arg+1,BUFFER_LEN);

    // If error
    if (chdir(arg) < 0)
      printf("cd: %s: %s\n", arg, strerror(errno));
  }
  else
    chdir("/home/junhao");
}

void execute(char **argv){
  // Handle "exit" command
  if (!strcmp(argv[0], "exit")){
    printf("Sea ya next time\n");
    free(argv);
    exit(EXIT_SUCCESS);
  }
  // Handle "cd" command
  else if (!strcmp(argv[0], "cd")){
    changeDir(argv[1]);
  }
  else{
    int f, status;
    f = fork();
    // If child process
    if (!f)
      if (execvp(argv[0], argv) < 0){
	// If error
	printf("%s: command not found\n", argv[0]);
	exit(EXIT_FAILURE);
      }
    else
      wait(&status);
  }
}

char ** parseInput(char *input, char *delim){
  int maxSize = 4; // Limit of the number of tokens in argv
  int size = 0; // Index counter
  char **argv = (char **)malloc(sizeof(char *));
  char *arg = strsep(&input, delim);
  char *tmp; // Used to trim whitespace

  for (; arg; arg = strsep(&input, delim), size++){
    // Reallocate when out of memory
    if (size == maxSize-2){
      maxSize *= 2;
      argv = (char **)realloc(argv, sizeof(char *)*maxSize);
    }

    // Trim leading and trailing white space from arg
    while (isspace(*arg))
      arg++;
    if (*arg){ // If *arg isn't all white space
      tmp = arg + strlen(arg) - 1;
      while (tmp > arg && isspace(*tmp))
	tmp--;
    }
    *(tmp+1) = 0;

    // Instantiate the array with tokens separated by `delim`
    argv[size] = arg;
  }
  // Append NULL to follow execvp() syntax
  argv[size] = NULL;

  return argv;
}

void shell(){
  char *input;
  char **argv = (char **)malloc(sizeof(char *));
  int count;

  while (1){
    printPrompt();
    fgets(input, BUFFER_LEN, stdin);

    // Separate commands (with ;) from input
    argv = parseInput(input, ";\n");

    // Execute each command
    count = 0;
    while (argv[count] && strcmp(argv[count], "")){
      execute(parseInput(argv[count], " "));
      count++;
    }
    
    free(argv);
  }
}

int main(){
  printf("Shellfish: Home of the Selfish\n");
  shell();
  return 0;
}
