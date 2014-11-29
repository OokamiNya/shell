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

void redirect(){
  int fd, oldfd;
  if (redirect_symbol[0] == '>'){
    oldfd = STDOUT_FILENO;
    if (redirect_symbol[1]) // ">>"
      fd = open(redirect_file, O_WRONLY|O_CREAT|O_APPEND, 0644);
    else // ">"
      fd = open(redirect_file, O_WRONLY|O_CREAT|O_TRUNC, 0644);
  }
  else if (redirect_symbol[0] == '<'){
    oldfd = STDIN_FILENO;
    fd = open(redirect_file, O_RDONLY, 0644);
  }
  else if (!strcmp(redirect_symbol,"|")){
  }
  else{
    printf("This should never happen.. See redirect(argv)\n"); // DEBUGGING
  }

  if (fd < 0)
    printError();
  else{
    if (dup2(fd, oldfd) < 0)
      printError();
    if (close(fd) < 0)
      printError();
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
    if (!argv[1])
      argv[1] = HOME;
    if (chdir(argv[1]) < 0)
      printf("cd: %s: %s\n", argv[1], strerror(errno));
  }
  else{
    int f, status;
    f = fork();
    if (f < 0){
      printError();
    }
    else if (!f){
      if (isRedirect)
	redirect();
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
  int maxSize = 1; // Limit of the number of tokens in argv
  int size = 0;
  char **argv = malloc(maxSize * sizeof *argv);
  char *arg = strsep(&input, delim);
  char *tmp; // Used to trim whitespace
  for (; arg; arg = strsep(&input, delim)){
    // Reallocate if out of memory
    if (size == maxSize){
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
	       !strcmp(arg,"<") || !strcmp(arg,"|")){
	redirect_symbol = arg;
	isRedirect = TRUE;
      }
      else{
	// Replace '~' with $HOME
	if (arg[0]=='~'){
	  char *tmp = malloc(strlen(HOME) + strlen(arg));
	  strcpy(tmp,HOME);
	  strcat(tmp,arg+1);
	  strcpy(arg,tmp);
	  free(tmp);
	}
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
      argv = parseInput(commands[count], " ");
      execute(argv);
      free(argv);
      if (isRedirect)
	isRedirect = FALSE;
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
