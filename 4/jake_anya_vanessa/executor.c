#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int execute(char s[256]) {

  char *s1 = s;
  char *s2;
  char *arg[256];
  
  int i = 0;
  int pipe = 0;
  while(s1) {
    s2 = strsep(&s1, " ");
    arg[i] = s2;
    if (!strcmp(arg[i], "|")){
      pipe = i;
      printf("%d\n", pipe);
    }
    i++;
  }
  arg[i] = 0;
  char * newstr;
  newstr = strsep(&(arg[i-1]), "\n");
  arg[i-1] = newstr;

  if(!strcmp(arg[0],"cd")){
    if (sizeof(arg) / sizeof(char *) > 1)
      chdir(arg[1]);
    else
      chdir(getenv("HOME"));
  }

  if (pipe){
    int fd1, fd2;
    char * cmd1 = arg[0];
    char * cmd2 = arg[pipe + 1];
    fd1 = open(stdin, 
  }

  /* piping things
  int fd;
  fd = open("loop.c", O_WRONLY | O_TRUNC);
  dup2(fd, STDOUT_FILENO);
  printf("Woo! This is working!");
  */

  else{
    execvp(arg[0], arg);
  }
  
  return 0;
}
