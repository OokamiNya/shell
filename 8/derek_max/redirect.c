#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "redirect.h"

int redir_out(char** args, char * dest) {
  //printf("Running %s and output to %s\n", args[0], dest);
  int fd;
  fd = open(dest, O_WRONLY | O_TRUNC | O_CREAT, 0666);
  dup2(fd,STDOUT_FILENO);
  execvp(args[0], args);
  return 0;
}

int redir_in(char** args, char * src) {
  //printf("Running %s by reading from %s\n", args[0], src);
  int fd;
  fd = open(src, O_RDONLY);
  dup2(fd,STDIN_FILENO);
  execvp(args[0], args);
  return 0;
}

int redir_pipe(char** args, char ** args2) {
  pid_t f2 = fork();
  int status;
  int w;
  if (f2==0){
    //printf("Piping command: output of %s will be input into %s\n",args[0],args2);
    redir_out(args,"tempfile");
  } else {
    w = wait(&status);
    int pos = 0;
    for (;pos<5;pos++){
      args2[pos]=args2[pos+1];
    }
    args2[5]='\0';

    redir_in(args2,"tempfile");
    return 1;
  }
  return 0;
}
