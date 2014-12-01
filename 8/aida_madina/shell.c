#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "parse.c"
#include "pipe.c"
#include "redirect.c"
int main() {
  char input[256];
  char **commands;
  char **args;
  char *flag_redir = NULL;
  int flag_redir_index = 0;
  int pid;
  int *status;
  siginfo_t *infop;
  char cwd[256];
  FILE * fout = NULL;
  int exit_val = 105;
  int file;
  
  while(1) {
    printf("seashell:%s$ ", getcwd(cwd, sizeof(cwd)));
    fgets(input, sizeof(input), stdin);
    input[strlen(input)-1]='\0';

    //Parsing commands
    commands = parse_commands(input);

    int i = 0;
    while (commands[i]) {
      
      args = parse_args(commands[i]);

      if (!strcmp(args[0],"exit")) {
	      exit(0);
      }
    
      else if (!strcmp(args[0],"cd")) {
        if (!args[1]) {
           chdir(getenv("HOME"));
        }
        chdir(args[1]);
      }
      int j = 0;
      while (args[j]) {
    	  if (!strcmp(args[j], ">")) {
          flag_redir_index = j;
    	    flag_redir = ">";
  	    }
        else if (!strcmp(args[j],">>")) {
          flag_redir_index = j;
          flag_redir = ">>";
        }
        else if (!strcmp(args[j],"<")) {
          flag_redir = "<";
        }
    	  j++;
      }  

      if (flag_redir) {
        if (!strcmp(flag_redir, "<")){ 
          redir_in(args);
        }
        else {
          redir_out(args, flag_redir_index);
        }
        flag_redir = 0;
        flag_redir_index = 0;
      }

      else {
        pid = fork();
        if(!pid) {
          execvp(args[0], args);  
          return WEXITSTATUS(exit_val);
        }
        waitid(P_PID, pid, infop, WEXITED);
      }
      i++;
      free(args);
    }
    free(commands);
  }
}


