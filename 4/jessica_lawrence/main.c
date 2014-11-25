#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shell.h"


int main () {
  char s[256];
  char **args = NULL;
  int argc;
  while (1) {
    prompt(s);
    argc = countargs(s);
    args = malloc((argc+1) * sizeof(char*));
    splitcmd(s,args);
    if (!strlen(s)) {
      //no command given, do nothing
    } else if(!strcmp(args[0],"exit")) {
      gtfo();
    } else if(!strcmp(args[0],"cd")) {
      if (chdir (args[1])){
        //will not take flags
        printf ("no such directory GET FKIN NOSCOPED REKT LMAO\n");
      }
    } else {
      int f = fork();
      if(f == 0) {
        docmd(args);
      } else {
        wait();
      }
    }
    free(args);
  }
  return 0;
}
