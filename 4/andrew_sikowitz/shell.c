#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>
#include <signal.h>

#include "shell.h"

#define pwd pw->pw_dir
int cpid;
char valid;

static void sighandler(int signo) {
  if (signo == SIGINT) {
    if (!cpid) {
      printf("\n");
      exit(2);
    }
    else
      valid = 0;
  }
  if (signo == SIGUSR1) {
    if (!cpid) {
      exit(1);
    }
    else
      valid = 0;
  }
}

char isRed(char c) {
  return c == '>' || c == '<';
}

char * clean(char * input) { //Alters string, so no malloc
  char * ph = input;
  while (*ph && *ph == ' ') {
    input++;
    ph++;
  }
  
  int i;
  int length = strlen(ph);
  char x = 0;
  //Shifts to left if more than 1 space
  for (i=0; i<length; i++) {
    if (ph[i] == ' ') {
      if (x) {
	strncpy(ph+i-1, ph+i, strlen(ph)+1);
	i--;
      }
      else
	x = 1;
    }
    else {
      x = 0;
    }
  }
  if (input[strlen(input)-1] == ' ')
    input[strlen(input)-1] = 0;

  return input;
}

int parse(char * input) {
  int c = 1;
  int i, j;
  char * ph = input; //Placeholder

  while (ph = strchr(ph, '>')) {
    c++;
    ph++;
  }
  ph = input;
  while (ph = strchr(ph, '<')) {
    c++;
    ph++;
  }
  ph = input;
  while (ph = strstr(ph, ">>")) {
    c--;
    ph+=2;
  }
  ph = input;

  char ** commands = (char **) malloc(sizeof(char **)*c);
  j = -2;
  for (i=0; i<c; i++) {
    j = j+2; //j starts at 0, 1, or 2 depending on:
    //First run starts at 0
    //Next runs starts at 1 to ignore > in for loop
    //Or 2 to ignore >>, etc. but include it in final string
    commands[i] = (char *) malloc(1024);
    for (; j<strlen(ph) && !isRed(ph[j]); j++) {}
    if (isRed(ph[j])) {
      if (ph[j+1] == '>') { //>>
	strncpy(commands[i], ph, j);
	commands[i][j-1] = 0;
	ph = ph+j;
	j = 0;
      }
      else { //> or <
	strncpy(commands[i], ph, j);
	commands[i][j] = 0;
	ph = ph+j;
	j = -1;
      }
    }
    else { //Allows for something like cat>file without spaces
      strncpy(commands[i], ph, j);
      commands[i][j] = 0;
      ph = ph+j;
      j = -1;
    }
  }

  /*for (i=0; i<c; i++) {
    printf("%s\n", commands[i]);
    }*/
  
  run(commands, c);
  
  return 0;
}

int separate(char * input) {
  char * step;
  char * clean_step = malloc(1024);

  while ((step = strsep(&input, ";"))) {
    strncpy(clean_step, step, 1024); //Duplicates step because clean alters string
    parse(clean(clean_step));
  }
  free(clean_step);

  return 0;
}

char ** sparse(char * command) {
  int c = 1;
  int i;
  int size = strlen(command);

  for (i=0; i<size; i++) {
    if (command[i] == ' ')
      c++;
  }
  
  char ** args = (char **) malloc(sizeof(char **) * (c+1)); //+1 for null

  for (i=0; i<c; i++) {
    args[i] = (char *) malloc(size+1); //+1 for nullstring
    strncpy(args[i], strsep(&command, " "), size);
    args[i][size] = 0;
  }

  args[c] = (char *) malloc(size+1); //Perhaps unnecessarily large
  args[c] = NULL;

  return args;
}

void ssfree(char ** starstar, int c) {
  int i;
  for (i=0; i<c; i++) {
    free(starstar[i]);
  }
  
  free(starstar);
}

int run(char ** commands, int c) {
  int f, status;
  
  if (!strncmp(commands[0], "cd", 2)) {
    char * command = clean(commands[0]);
    char ** args = sparse(command);
    ssfree(commands, c);

    if (chdir(args[1]))
      printf("%s\n", strerror(errno));
    ssfree(args, 2); 
  }
  else if (!strncmp(commands[0], "exit", 4)) {
    ssfree(commands, c);
    kill(getppid(), SIGUSR1);
    exit(9);
  }   
  else {
    f = fork();
    if (f) {
      wait(&status);
      //printf("%d\n", WTERMSIG(status));
    }
    else {
      int i, fd;
      char * file;
      for (i=c-1; i>=0; i--) {
	if (commands[i][0] == '>' && commands[i][1] == '>') {
	  file = clean(commands[i]+2); //Ignores >>
	  fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	  dup2(fd, STDOUT_FILENO);
	}
	else if (commands[i][0] == '>') {
	  file = clean(commands[i]+1); //Ignores >
	  fd = open(file, O_CREAT | O_WRONLY, 0644);
	  dup2(fd, STDOUT_FILENO);
	}
	else if (commands[i][0] == '<') {
	  file = clean(commands[i]+1);
	  fd = open(file, O_RDONLY);
	  dup2(fd, STDIN_FILENO);
	}
	else {
	  char ** args = sparse(clean(commands[i]));
	  ssfree(commands, c);
	  if (execvp(args[0], args))
	    printf("%s\n", strerror(errno));
	  exit(-1);
	}
      }
      char ** args = sparse(commands[0]); //Default
      //Should only occur if bad input (allows for error message)
      ssfree(commands, c);
      if (execvp(args[0], args))
	printf("%s\n", strerror(errno));
      exit(-1);
    }
  }
  
  return 0;
}

void main_run(char * hostname, char * cwd, struct passwd *pw) {
  int status;
  char cwd_new[4096];
  int pipefd[2];
  char * check;
  pipe(pipefd);
  cpid = fork();
  valid = 1;
  
  if (cpid) {
    close(pipefd[1]);
    *cwd_new = 0;
    while (*cwd_new == 0 && valid)
      read(pipefd[0], &cwd_new, 4096);
    close(pipefd[0]);
    kill(cpid, SIGUSR1);
    waitpid(cpid, &status, 0);
    
    if (WIFEXITED(status)) {
      switch(WEXITSTATUS(status)) {
      case 9:
        exit(0);
      case 10:
	printf("Something went wrong\n");
      }
    }
    else
      printf("Error exiting\n");

    if (chdir(cwd_new) && valid)
      printf("%s\n", strerror(errno));
  }
  else {
    close(pipefd[0]);
    getcwd(cwd, 4096);
    gethostname(hostname, 4096);
    getpwuid(getuid());
    check = strstr(cwd, pwd);
    if (check) {
      cwd = cwd + strlen(pwd) - 1;
      *cwd = '~';
    }
    printf("%s@%s:%s$ ", getlogin(), hostname, cwd);
    char * input = (char *) malloc(1024);
    fgets(input,1024,stdin);
    input[strlen(input)-1] = 0; //Removes newline
	
    separate(input);
    free(input);
    getcwd(cwd, 4096);
    if ((write(pipefd[1], cwd, 4096)) == -1)
      printf("%s\n", strerror(errno));
    close(pipefd[1]);
    pause();
    exit(10); //Shouldn't run
  }
}
      
int main() {
  //Main program

  signal(SIGINT, sighandler);
  signal(SIGUSR1, sighandler);
  
  printf("PID: %d\n", getpid());
  //printf("Login: %s\n", getlogin());

  char hostname[4096];
  char cwd[4096];
  char * cwdp;
  getcwd(cwd, 4096);
  gethostname(hostname, 4096);
  struct passwd *pw = getpwuid(getuid());

  while (1) {
    main_run(hostname, cwd, pw);
  }
}
