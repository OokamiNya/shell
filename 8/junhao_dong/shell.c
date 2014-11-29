/* TODO:
   optimize (check for excessive loops ex. strlen)
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
int redirect_index;
char *redirect_symbol;

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

void redirect(char *redirect_file){
  int fd, oldfd;
  if (redirect_symbol[0] == '>'){
    oldfd = STDOUT_FILENO;
    if (redirect_symbol[1]) // ">>"
      fd = open(redirect_file, O_WRONLY|O_CREAT|O_APPEND, 0644);
    else // ">"
      fd = open(redirect_file, O_WRONLY|O_CREAT|O_TRUNC, 0644);
  }
  else{ // "<"
    oldfd = STDIN_FILENO;
    fd = open(redirect_file, O_RDONLY, 0644);
  }
  // Redirect
  if (fd < 0)
    printError();
  else{
    if (dup2(fd, oldfd) < 0)
      printError();
    if (close(fd) < 0)
      printError();
  }
}

// No error checking for close()
void executePipe(char **argv){
  int f, status, fd[2]; // fd[0] = in; fd[1] = out
  if (pipe(fd) < 0)
    printError();
  argv[redirect_index] = NULL;

  f = fork();
  if (f < 0)
    printError();
  else if (!f){ // Child; in pipe
    close(fd[0]);
    if (dup2(fd[1], STDOUT_FILENO) < 0)
      printError();
    //executeMisc(argv);
  }
  else{ // Parent; out pipe
    wait(&status);
    close(fd[1]);
    if (dup2(fd[0], STDIN_FILENO) < 0)
      printError();
    //executeMisc(&argv[redirect_index+1]);
    argv = &argv[redirect_index+1];
  }
  execvp(argv[0], argv);
  printf("%s: command not found\n", argv[0]);
  exit(EXIT_FAILURE);
}

// Handles everything besides `cd` and `exit` or `quit`
void executeMisc(char **argv){
  int f, status;
  f = fork();
  if (f  < 0)
    printError();
  else if (!f){
    if (isRedirect){
      // Piping
      if (redirect_symbol[0] == '|'){
	executePipe(argv);
      }
      // Redirecting
      redirect(argv[redirect_index+1]);
      argv[redirect_index] = NULL;
    }
    execvp(argv[0], argv);
    printf("%s: command not found\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  else{
    wait(&status);
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
    executeMisc(argv);
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
      // Check if redirect
      if (!strcmp(arg,">") || !strcmp(arg,">>") || \
	       !strcmp(arg,"<") || !strcmp(arg,"|")){
	isRedirect = TRUE;
	redirect_symbol = arg;
	redirect_index = size;
      }
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
    // Execute each command
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
