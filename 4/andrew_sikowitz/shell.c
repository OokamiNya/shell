#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#include "shell.h"

#define pwd pw->pw_dir

char isRed(char c) {
  return c == '>' || c == '<' || c == '|';
}

char * clean(char * input) {
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
  for (i=0; i<strlen(input); i++) {
    if (isRed(input[i]))
      c++;
  }
  
  char ** commands = (char **) malloc(1024*c);
  char * ph = input;
  j = -2;
  for (i=0; i<c; i++) {
    j = j+2; //j starts at 0, 1, or 2 depending on:
    //First run starts at 0
    //Next runs starts at 1 to ignore >, or 2 to ignore >>, etc.
    commands[i] = (char *) malloc(1024);
    for (; j<strlen(ph) && !isRed(ph[j]); j++) {}
    if (ph[j-1] == ' ') {
      strncpy(commands[i], ph, j);
      commands[i][j] = 0;
      ph = ph+j;
      j = -1;
    }
    else if (ph[j-2] == ' ') {
      strncpy(commands[i], ph, j-1);
      commands[i][j-1] = 0;
      ph = ph+j-1;
      j = 0;
    }
    else { //Allows for something like cat>file without spaces
      strncpy(commands[i], ph, j);
      commands[i][j] = 0;
      ph = ph+j;
      j = -1;
    }
  }

  for (i=0; i<c; i++) {
    printf("%s\n", commands[i]);
  }

  //run(commands, c);
  
  return 0;
}

int separate(char * input) {
  char * step;
  while ((step = strsep(&input, ";"))) {
    parse(step);
  }

  return 0;
}
/*
char ** sparse(char * command) {
  int c = 0;
  int i;
  for (i=0; i<c; i++) {

  }
}

int run(char ** commands, int c) {
  int f, status;
  f = fork();
  if (f) {
    wait(&status);
  }
  else {
    int i;
    for (i=c-1; i>=0; i--) {
      
      char ** func = sparse(commands[0]);
*/
int main() {
  //Main program
  printf("PID: %d\n", getpid());
  //printf("Login: %s\n", getlogin());
  char hostname[4096];
  char cwd[4096];
  char * cwdp;
  getcwd(cwd, 4096);
  gethostname(hostname, 4096);
  struct passwd *pw = getpwuid(getuid());
  //printf("Home Directory: %s\n", pwd);
  //printf("CWD: %s\n", cwd);
  //printf("Hostname: %s\n", hostname);
  while (1) {
    getcwd(cwd, 4096);
    gethostname(hostname, 4096);
    getpwuid(getuid());
    cwdp = strstr(cwd, pwd) + strlen(pwd) - 1;
    *cwdp = '~';
    printf("%s@%s:%s$ ", getlogin(), hostname, cwdp);
    char * input = (char *) malloc(1024);
    fgets(input,1024,stdin);
    input[strlen(input)-1] = 0; //Removes newline
    separate(input);
    free(input);
  }
}
