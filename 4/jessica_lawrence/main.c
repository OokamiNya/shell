#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "shell.h"


int main () {
  char raw [256];
  char clean[256];
  char **args = NULL;
  char **cmds = NULL;
  int argc;
  int cmdc;
  int i;
  int redir_in;
  int redir_out;
  while (1) {
    redir_in = 0;
    redir_out = 0;
    prompt(raw);
    killspaces(raw,clean);
    cmdc = count(clean, ";");
    cmds = malloc(cmdc * sizeof(char*));
    splitinput(clean,cmds);

    i = 0;
    while(i<cmdc) {
      argc = count(cmds[i], " ");
      args = malloc((argc+1) * sizeof(char*)); //+1 for NULL
      
      if (strchr (cmds[i], '|')) {
        char* temp = cmds[i];
        strsep(&temp, "|");
        //printf ("%s\n", cmds[i]);
        //printf ("%s", temp);
        //execvp ("ls", "ls");
        splitcmd(cmds[i],args);
        redir_out = open("foo", O_WRONLY|O_CREAT, 0666);
        int f1 = fork();
        if (f1 == 0) {
          docmd(args,redir_in,redir_out);
        } else {
          wait(1);
        }
        splitcmd(temp,args);
        redir_out = 0;
        redir_in = open("foo", O_RDONLY, 0666);
        int f2 = fork();
        if (f2 == 0) {
          docmd(args,redir_in,redir_out);
        } else {
          wait(1);
        }
        int f = fork();
        if (f == 0) {
          execlp ("rm", "rm", "foo", NULL);
        } else {
          wait(1);
        }
	
      }
      else {
        if (strchr (cmds[i], '>')) {
          char* temp = cmds[i];
          strsep(&temp, ">");
          redir_out = open(temp, O_WRONLY|O_CREAT, 0666);
        }
        if (strchr (cmds[i], '<')) {
          char* temp = cmds[i];
          strsep(&temp, "<");
          redir_in = open(temp, O_RDONLY, 0666);
        }
      
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
            docmd(args,redir_in,redir_out);
          } else {
            wait(1);
          }
        }
      }
      i++;
    }

    free(args);
    free(cmds);
  }
  return 0;
}
