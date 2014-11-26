#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "shell.h"

<<<<<<< HEAD
#define TRUE 1
#define FALSE 0

#define BUFFER_LEN 256
#define HOME getenv("HOME")

/* TODO==========
   konami easter eggs?? scrolling fish idk..
   ^ on exit maybe:  http://www.ascii-art.de/ascii/def/fish.txt
   
   REAL TODO==========
=======
#define BUFFER_LEN 256
#define HOME getenv("HOME")

/* TODO:
   konami easter eggs?? scrolling fish idk..
   ^ on exit maybe:  http://www.ascii-art.de/ascii/def/fish.txt
   
   REAL TODO:
   bug: prompt ~ replacing $HOME when at $HOME or a path with fewer chars
>>>>>>> 8b4c16d2f3cacbfb7c1a8b49d47617a6d917b115
   redirecting
   pipes

   add color to prompt, customize, bold
   sighandler - catch sigint, ctrl d for eof
<<<<<<< HEAD
   command history and navigation
   directory stack/linked list
   'cd - n' goes to nth previous dir
   tab completion!!
   method abstraction, utils.c file maybe (for parsing, trim, etc)

   better errors?
     errno's from getcwd
     syntax errors (cmd:";;;;")
     no such file or dir (redirecting)
   final checks to optimize code + memory usage
     perhaps ignore white space rather than trim?
     excessive looping (strlen, other str fxns)
   design.txt, edit header file, readme

   NOTES========== 
   trims excess white space and ';'
   does not print all errors (ex: error from getcwd, syntax error)
     ignores multiple ;'s rather than give syntax error (as in bash)
 */

int isRedirect = FALSE;

void printPrompt(){
  char *cwd = getcwd(cwd,0); // Dynamically allocated
=======
   command history, up arrow (or ctrl p) for previous command
   'cd -' goes to previous directory
   method abstraction, utils.c file maybe (for parsing, trim, etc)
   tab completion
   better errors?
       syntax errors (cmd:";;;;")
   final checks to optimize code + memory usage
       perhaps ignore white space rather than trim?
       excessive looping (strlen, other str fxns)
       unnecessarily large buffer allocations
       memory leaks
   design.txt, edit header file, readme

   NOTES: (to be moved to README)
   trim excess white space and ';'
   ignores multiple ;'s rather than give syntax error (as in bash)
   does not print all errors (ex: error from getcwd, syntax error)
 */


void printPrompt(){
  char *cwd = getcwd(cwd,0); // Dynamically allocated

>>>>>>> 8b4c16d2f3cacbfb7c1a8b49d47617a6d917b115
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
<<<<<<< HEAD
=======

>>>>>>> 8b4c16d2f3cacbfb7c1a8b49d47617a6d917b115
  free(cwd);
}

void changeDir(char *arg){
  // If `cd` is given an argument
  if (arg){
    // Replace '~' with $HOME
    if (arg[0]=='~'){
<<<<<<< HEAD
      char *tmp = malloc(strlen(HOME) + strlen(arg));
      strcpy(tmp,HOME);
      strcat(tmp,arg+1); // strncat
      strcpy(arg,tmp);
      free(tmp);
    }
=======
      char *tmp = (char *)malloc(BUFFER_LEN);
      strcpy(tmp,HOME);
      strcat(tmp,arg+1);
      strcpy(arg,tmp);
      free(tmp);
    }

>>>>>>> 8b4c16d2f3cacbfb7c1a8b49d47617a6d917b115
    // If error
    if (chdir(arg) < 0)
      printf("cd: %s: %s\n", arg, strerror(errno));
  }
  else
    chdir(HOME);
}

void execute(char **argv){
  // Handle "exit" command
<<<<<<< HEAD
  if (isRedirect)
    redirect();
  else if (!strcmp(argv[0], "exit")){
=======
  if (!strcmp(argv[0], "exit")){
>>>>>>> 8b4c16d2f3cacbfb7c1a8b49d47617a6d917b115
    printf("Sea ya next time\n");
    free(argv);
    exit(EXIT_SUCCESS);
  }
  // Handle "cd" command
  else if (!strcmp(argv[0], "cd")){
    changeDir(argv[1]);
  }
  // Handle non-null or non-whitespace commands
  else{
    int f, status;
    f = fork();
    // If child process
    if (!f){
      if (execvp(argv[0], argv) < 0){
	// If error
	printf("%s: command not found\n", argv[0]);
	exit(EXIT_FAILURE);
      }
    }
    else
      wait(&status);
  }
}

// Free dynamically allocated memory after use
char ** parseInput(char *input, char *delim){
  int maxSize = 4; // Limit of the number of tokens in argv
  int size = 0; // Index counter
<<<<<<< HEAD
  char **argv = malloc(sizeof *argv);
  char *arg = strsep(&input, delim);
  char *tmp; // Used to trim whitespace
=======
  char **argv = (char **)malloc(sizeof(char *));
  char *arg = strsep(&input, delim);
  char *tmp; // Used to trim whitespace

>>>>>>> 8b4c16d2f3cacbfb7c1a8b49d47617a6d917b115
  // Separate input by `delim` into arg tokens
  for (; arg; arg = strsep(&input, delim)){
    // Reallocate when out of memory
    if (size == maxSize-2){
      maxSize *= 2;
<<<<<<< HEAD
      argv = realloc(argv, maxSize*sizeof *argv);
    }
=======
      argv = (char **)realloc(argv, sizeof(char *)*maxSize);
    }

>>>>>>> 8b4c16d2f3cacbfb7c1a8b49d47617a6d917b115
    // Trim leading white space and ';' from arg
    while (isspace(*arg) || *arg==';')
      arg++;
    // If arg isn't all white space or ';'
    if (*arg){
      // Trim trailing white space and ';'
      tmp = arg + strlen(arg) - 1;
      while (tmp > arg && (isspace(*tmp) || *arg==';'))
	tmp--;
<<<<<<< HEAD
      *(tmp+1) = '\0';
      // Instantiate the array
      if (!strcmp(arg,"<<") || !strcmp(arg,"<") || !strcmp(arg,">>") \ 
	  || !strcmp(arg,">") || !strcmp(arg,"|") || !strcmp(arg,"<") !strcmp(arg,"tee"))
	isRedirect = TRUE;
=======
      *(tmp+1) = 0;

      // Instantiate the array
>>>>>>> 8b4c16d2f3cacbfb7c1a8b49d47617a6d917b115
      argv[size] = arg;
      size++;
    }
  }
  // Append NULL to follow execvp() syntax
  argv[size] = NULL;
<<<<<<< HEAD
=======

>>>>>>> 8b4c16d2f3cacbfb7c1a8b49d47617a6d917b115
  return argv;
}

void shell(){
  char *input;
<<<<<<< HEAD
  char **argv = malloc(sizeof *argv);
  int count;
  while (1){
    printPrompt();
    fgets(input, BUFFER_LEN, stdin);
    // Separate commands (with ;) from input
    argv = parseInput(input, ";");
    // Execute each command
    count = 0;
    while (argv[count]){
      isRedirect = FALSE;
      execute(parseInput(argv[count], " "));
      count++;
    }
=======
  char **argv = (char **)malloc(sizeof(char *));
  int count;

  while (1){
    printPrompt();
    fgets(input, BUFFER_LEN, stdin);

    // Separate commands (with ;) from input
    argv = parseInput(input, ";");

    // Execute each command
    count = 0;
    while (argv[count]){
      execute(parseInput(argv[count], " "));
      count++;
    }
    
>>>>>>> 8b4c16d2f3cacbfb7c1a8b49d47617a6d917b115
    free(argv);
  }
}

int main(){
  printf("Shellfish: Home of the Selfish\n");
  shell();
  return 0;
}
