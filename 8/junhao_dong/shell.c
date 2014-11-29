#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include "shell.h"

#define TRUE 1
#define FALSE 0

#define BUFFER_LEN 512
#define HOME getenv("HOME")

int isPipe = FALSE;
int redir_in = FALSE; // "<"
int redir_out = FALSE; // ">"
char *inFile, *inSymbol, *outFile, *outSymbol;

int f, status; // for fork(), wait()

char **commands; // split by `;`
char **argv; // split by ` `

/*
static void sigHandler(int signo){
  if (signo == SIGINT){
    if (!f)
      exit(EXIT_FAILURE); // success?
  }
}
*/

void printPrompt(){
  char *cwd = 0;
  cwd = getcwd(cwd,0);
  // Replace $HOME with '~'
  if (strstr(cwd,HOME)){
    int homeLen = strlen(HOME);
    int relPathLen = strlen(cwd)-homeLen+1;
    strncpy(cwd, &cwd[homeLen-1], relPathLen);
    cwd[0] = '~';
    cwd[relPathLen] = '\0';
  }
  printf("%s: %s$\n", getenv("USER"), cwd);
  printf("><((((º> ");
  free(cwd);
}

// Trim white space and ';' from *str
char * trimSpace(char *str){
  char *tmp;
  while (isspace(*str) || *str==';')
    str++;
  if (*str){
    tmp = str + strlen(str) - 1;
    while (tmp > str && (isspace(*tmp) || *str==';'))
      tmp--;
    *(tmp+1) = '\0';
  }
  return str;
}

void redirect(){
  int fd, oldfd;
  if (redir_out){
    oldfd = STDOUT_FILENO;
    if (outSymbol[1]) // ">>"
      fd = open(outFile, O_WRONLY|O_CREAT|O_APPEND, 0644);
    else // ">"
      fd = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    // Redirect
    if (fd < 0)
      printf("ERROR: %s\n", strerror(errno));
    else{
      dup2(fd, oldfd);
      close(fd);
    }
  }
  if (redir_in){ // "<"
    oldfd = STDIN_FILENO;
    fd = open(inFile, O_RDONLY, 0644);
    // Redirect
    if (fd < 0)
      printf("ERROR: %s\n", strerror(errno));
    else{
      dup2(fd, oldfd);
      close(fd);
    }
  }
}

void executePipe(){
  int pipeIndex, fd[2]; // fd[0] = in; fd[1] = out
  pipe(fd);
  // Find pipe index
  pipeIndex = 0;
  while (*argv[pipeIndex] != '|')
    pipeIndex++;

  f = fork();
  if (!f){ // Child; in pipe
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    argv[pipeIndex] = NULL;
  }
  else{ // Parent; out pipe
    wait(&status);
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    argv = &argv[pipeIndex+1];
  }
  //executeMisc(argv);
  execvp(argv[0], argv);
  printf("%s: command not found\n", argv[0]);
  exit(EXIT_FAILURE);
}

void executeMisc(){
  f = fork();
  if (!f){
    // Piping
    if (isPipe)
      executePipe();
    // Redirecting
    else if (redir_in || redir_out)
      redirect();
    execvp(argv[0], argv);
    printf("%s: command not found\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  else{
    wait(&status);
  }
}

void execute(){
  // `exit` or `quit`
  if (!strcmp(argv[0], "exit") || !strcmp(argv[0], "quit")){
    printf("Sea ya next time\n");
    free(argv);
    free(commands);
    exit(EXIT_SUCCESS);
  }
  // `cd`
  else if (!strcmp(argv[0], "cd")){
    if (!argv[1])
      argv[1] = HOME;
    if (chdir(argv[1]) < 0)
      printf("cd: %s: %s\n", argv[1], strerror(errno));
  }
  else
    executeMisc();
}

// Returns dynamically allocated memory
char ** parseInput(char *input, char *delim){
  int maxSize = 1; // Limit of the number of tokens in argv
  int size = 0;
  char **argv = malloc(maxSize * sizeof *argv);
  char *arg = strsep(&input, delim);
  for (; arg; arg = strsep(&input, delim)){
    // Reallocate if out of memory
    if (size == maxSize){
      maxSize *= 2;
      argv = realloc(argv, maxSize * sizeof *argv);
    }
    // Trim white space
    arg = trimSpace(arg);
    if (*arg){
      // Check if redirect or pipe
      if (*arg == '>'){
	redir_out = TRUE;
	outSymbol = arg;
      }
      else if (*arg == '<'){
	redir_in = TRUE;
	inSymbol = arg;
      }
      else if (*arg == '|'){
	isPipe = TRUE;
	argv[size] = "|";
	size++;
      }
      else if (redir_out && !outFile) outFile = arg;
      else if (redir_in && !inFile) inFile = arg;
      else{
	// Replace '~' with $HOME
	if (arg[0]=='~'){
	  char tmp[BUFFER_LEN]; // strlen(HOME) + strlen(arg)
	  strcpy(tmp,HOME); // buffer overflow check later
	  strcat(tmp,arg+1);
	  strcpy(arg,tmp);
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
  int count;
  while (1){
    printPrompt();
    fgets(input, BUFFER_LEN, stdin);
    commands = parseInput(input, ";");
    count = 0;
    // Execute each command
    while (commands[count]){
      argv = parseInput(commands[count], " ");
      execute();
      free(argv);
      count++;
      // Reset globals
      isPipe = FALSE;
      redir_in = FALSE;
      redir_out = FALSE;
      inFile = 0;
      outFile = 0;
    }
    free(commands);
  }
}

int main(){
  //signal(SIGINT, sigHandler);
  printf("Welcome to Shellfish!\n");
  shell();
  return 0;
}
