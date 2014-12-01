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

char is_control_char(char c) {
  return c==';' || c=='>' || c=='<' || c=='|';
}

char* killspaces(char* src, char* dest) {
  int len = strlen(src);
  char* tokill = malloc(len * sizeof(char));
  int i,j;
  for(i=0;i<len;i++) {tokill[i]=0;}

  if(src[0]==' ') {tokill[0]=1;}

  for(i=1;i<len;i++) {
    if(src[i]==' ') {
      if(src[i-1]==' ') {
        tokill[i]=1;
      }
    } else if(is_control_char(src[i])) {
      j=i-1;
      while(src[j]==' ' && j>0) {
        tokill[j]=1;
        j--;
      }
      j=i+1;
      while(src[j]==' ' && j<len) {
        tokill[j]=1;
        j++;
      }
    }
  }

  i=0;
  j=0;
  while(i<len) {
    if(tokill[i]) {
      i++;
    } else {
      dest[j] = src[i];
      i++;
      j++;
    }
  }
  dest[j-1]=0;

  free(tokill);
  return dest;
}

int count (char*s, char *c) {
  char* temp = malloc(256*sizeof(char));
  char* starttemp = temp;
  strncpy(temp, s, 256);
  int argc = 0;
  while(strsep(&temp, c)) {
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

void docmd(char** args, int redir_in, int redir_out) {
  if(redir_in) {
    dup2(redir_in, STDIN_FILENO);
    close(redir_in);
  }
  if(redir_out) {
    dup2(redir_out, STDOUT_FILENO);
    close(redir_out);
  }
  execvp (args[0], args);
  printf ("LMFAO no such command XDDDDDD q:^)-k\n");
  exit(0);
}

void gtfo() {
  printf("bye :^(\n");
  exit(0);
}


