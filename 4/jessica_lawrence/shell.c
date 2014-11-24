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

void splitcmd(char* s, char** args) {
  char *c = s;
  char *t;
  int i = 0; 
  
  c = strsep (&c, "\n");
  
  while ((t = strsep(&c, " "))) {
    args[i] = t;
    i++;
  }

  args [i] = 0;
}

void docmd(char** args) {
  execvp (args[0], args);
  printf ("LMFAO no such command ur a dum@$$ XDDDDDD :^)\n");
  exit(0);
}

void gtfo() {
  exit(0);
}

