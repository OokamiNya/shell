#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "redirect.h"

int redir_out(char** args, char * dest) {
  int fd;
  //printf("--- Writing to file: %s\n", dest);
  fd = open(dest, O_WRONLY | O_TRUNC | O_CREAT, 0666);
  dup2(fd,STDOUT_FILENO);
  execvp(args[0], args);
  return 0;
}

int redir_in(char** args, char * src, int loc) {
  int fd;
  fd = open(src, O_RDONLY);
  //char content[256];
  //int b;
  //b = read(fd, content,sizeof(content));
  //args[loc]=content;
  printf("--- Reading from file: %s\n", src);
  //printf("--- Load file contents into args[%d]: \n%s\n\n",loc,content);
  //printf("--- args[0] = %s, args[1] = %s \n\n", args[0], args[1]);
  dup2(fd,STDIN_FILENO);
  execvp(args[0], args);
  return 0;
}
/*
int pipe() {
  return 0;
}
*/
