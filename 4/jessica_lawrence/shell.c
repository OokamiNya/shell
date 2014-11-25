#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

void prompt(char* s) {
  printf ("[ :^) ]  ");
  fgets (s, 256, stdin);
}

int countcmds(char* s) {
  char* temp = malloc(256*sizeof(char));
  char* starttemp = temp;
  strncpy(temp, s, 256);
  int argc = 0;
  while(strsep(&temp, ";")) {
    argc++;
  }
  free(starttemp);
  return argc;
}

int countargs(char* s) {
  char* temp = malloc(256*sizeof(char));
  char* starttemp = temp;
  strncpy(temp, s, 256);
  int argc = 0;
  while(strsep(&temp, " ")) {
    argc++;
  }
  free(starttemp);
  return argc;
}

void splitinput(char* s, char** cmds) {
  char *c = s;
  char *t;
  int i = 0; 
  
  while ((t = strsep(&c, ";"))) {
    cmds[i] = t;
    i++;
  }
}

void splitcmd(char* s, char** args) {
  char *c = s;
  char *t;
  int i = 0; 
  
  c = strsep(&c,"\n");
  
  while ((t = strsep(&c, " "))) {
    args[i] = t;
    i++;
  }

  args [i] = 0;
}

void docmd(char** args) {
  execvp (args[0], args);
  printf ("LMFAO no such command XDDDDDD q:^)-k\n");
  exit(0);
}

void gtfo() {
  printf("bye :^(\n");
  exit(0);
}


