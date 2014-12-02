#include "pipe.h"

int pipes(char **args, int index, int end) {
  int file;
  if (!strcmp(args[index],"|")) {
    file = open("pipes_temp", O_CREAT | O_WRONLY| O_TRUNC, 0644);
    close(file);    
    int pid = fork();
    if (!pid) {
      dup2(file, STDOUT_FILENO);
      int n;
      char *exec_args[10];
      for(n = 0; n < index; n++) {
	exec_args[n] = args[n];
      }
      exec_args[n] = NULL;
      execvp(args[0], exec_args);
      
      dup2(STDIN_FILENO, file);
      for(n = index + 1; n < end; n++) {
	exec_args[n - index - 1] = args[n];
      }
      exec_args[n] = NULL;
      execvp(args[index + 1], exec_args);

      //execvp("rm" , "rm pipes_temp\0");
      return 0;
    }
  }
  return 0;
}
