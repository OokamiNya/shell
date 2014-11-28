/* TODO==========
   redirecting - handle multiple (sort < file.txt > sortedfiles.txt)

   pipes
   feature: ls ~ (~ for all commands)
   add color to prompt, customize, bold
   sighandler - catch sigint, ctrl d for eof
   command history, up arrow (or ctrl p) for previous command
   directory stack/linked list
   'cd - n' goes to nth previous dir ; dir stack/linklist
   tab completion
   <<< here string; << here doc
   header file, readme

   NOTES========== 
   ignores everything after first token after redir symbol
   trims excess white space and ';'
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

#define printError() printf("ERROR: %s\n", strerror(errno))

int isRedirect = FALSE;
char *redirect_symbol;
char *redirect_file;


void printPrompt(){
  char *cwd = 0;
  cwd = getcwd(cwd,0);
  if (!cwd){
    printError();
    return;
  }
  // Replace $HOME with '~'
  if (strstr(cwd,HOME)){
    int homeLen = strlen(HOME);
    int relPathLen = strlen(cwd)-homeLen+1;
    strncpy(cwd, &cwd[homeLen-1], relPathLen);
    cwd[0] = '~';
    cwd[relPathLen] = '\0';
  }
  printf("%s$\n", cwd);
  printf("><((((º> ");
  free(cwd);
}

void changeDir(char *arg){
  if (arg){
    // Replace '~' with $HOME
    if (arg[0]=='~'){
      char *tmp = malloc(strlen(HOME) + strlen(arg));
      strcpy(tmp,HOME);
      strcat(tmp,arg+1);
      strcpy(arg,tmp);
      free(tmp);
    }
  }
  else{
    arg = HOME;
  }
  if (chdir(arg) < 0)
    printf("cd: %s: %s\n", arg, strerror(errno));
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
  // `exit` or `quit`
  if (!strcmp(argv[0], "exit") || !strcmp(argv[0], "quit")){
    printf("Sea ya next time\n");
    free(argv);
    exit(EXIT_SUCCESS);
  }
  // `cd`
  else if (!strcmp(argv[0], "cd")){
    changeDir(argv[1]);
  }
  else{
    int f, status;
    f = fork();
    if (f < 0)
      printError();
    else if (!f){
      if (isRedirect){
 	redirect();
      }
      if (execvp(argv[0], argv) < 0){
	printf("%s: command not found\n", argv[0]);
	exit(EXIT_FAILURE);
      }
    }
    else{
      wait(&status);
    }
  }
}

// Returns dynamically allocated memory
char ** parseInput(char *input, char *delim){
  int size, maxSize = 4; // Limit of the number of tokens in argv
  char **argv = malloc(maxSize * sizeof *argv);
  char *arg = strsep(&input, delim);
  char *tmp; // Used to trim whitespace
  for (size = 0; arg; arg = strsep(&input, delim)){
    // Reallocate if out of memory
    if (size == maxSize-2){
      maxSize *= 2;
      argv = realloc(argv, maxSize * sizeof *argv);
    }
    // Trim white space and ';' from arg
    while (isspace(*arg) || *arg==';')
      arg++;
    if (*arg){
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
    printPrompt();
    fgets(input, BUFFER_LEN, stdin);

    commands = parseInput(input, ";");

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
