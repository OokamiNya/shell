#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int f;

int execute(char s[256]) {

  char *s1 = s;
  char *s2;
  char *arg[256];
  
  int i = 0;
  int pipe = 0;
  while(s1) {
    //generates arg (array of arguments)
    s2 = strsep(&s1, " ");
    arg[i] = s2;
    if (!strcmp(arg[i], "|")){
      //if it's a pipe, get the location of the pipe
      pipe = i;
      printf("%d\n", pipe);
    }
    i++;
  }

  /*
    minor cleaning up
    last argument = NULL for execvp to work
    newstr is the new second-to-last argument with \n removed
  */

  arg[i] = 0;
  char * newstr;
  newstr = strsep(&(arg[i-1]), "\n");
  arg[i-1] = newstr;

  //handles cd
  if(!strcmp(arg[0],"cd")){
    if (sizeof(arg) / sizeof(char *) > 1)
      chdir(arg[1]);
    else
      chdir(getenv("HOME"));
  }
  
  //handles exit
  if(!strcmp(arg[0], "exit")){
    exit(0);
  }
    

  //handle(?) pipe
  if (pipe){
    int fd1, fd2;
    char * cmd1 = arg[0];
    char * cmd2 = arg[pipe + 1];
    //fd1 = open(stdin, 
    printf("YOU GOTTA FIX PIPE\n");
  }

  else{
    f = fork();
    //if it's a child, execvp the args
    if (!f){
      execvp(arg[0], arg);
      exit(0);
    }
    //if it's a parent, wait for child
    else{
      wait(f);
      //resets the f to 0 so it doesn't think there's a kid
      f = 0;
    }
  }
  
  return 0;
}

char *replace_string(char *str, char *orig, char *rep){

  static char buffer[4096];
  char *p;

  if(!(p = strstr(str, orig)))
    return str;

  strncpy(buffer, str, p-str);
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return buffer;
}
