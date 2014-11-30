#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shell.h"


int main () {
  char raw[256];
  char clean[256];
  char **args = NULL;
  char **cmds = NULL;
  int argc;
  int cmdc;
  int i;
  while (1) {
    prompt(raw);
    killspaces(raw,clean);
    cmdc = countcmds(clean);
    cmds = malloc(cmdc * sizeof(char*));
    splitinput(clean,cmds);
    i = 0;
    while(i<cmdc) {
      argc = countargs(cmds[i]);
      args = malloc((argc+1) * sizeof(char*)); //+1 for NULL
      splitcmd(cmds[i],args);
      if (!strlen(cmds[i])) {
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
          wait(1);
        }
        }
      i++;
    }
    free(args);
    free(cmds);
  }
  return 0;
}
