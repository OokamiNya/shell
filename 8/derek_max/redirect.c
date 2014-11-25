#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "redirect.h"

int output(char** args, char * dest) {
  int fd;
  //printf("--- Writing to file: %s\n", dest);
  fd = open(dest, O_WRONLY | O_TRUNC | O_CREAT, 0666);
  dup2(fd,1);
  execvp(args[0], args);
  return 0;
}
/*
int input() {
  return 0;
}

int pipe() {
  return 0;
}
*/
