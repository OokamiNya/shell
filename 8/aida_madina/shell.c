#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>

int count_commands(char input[256]);
int count_args(char *command);

int main() {
  char input[256];
  char *comm_array[10];
  char *command;
  int num_commands;
  int pid;
  int *status;
  siginfo_t *infop;
  char cwd[256];
  int flag_redir = 0;
  FILE * fout = NULL;
  int exit_val = 105;
  int flag_redir_type = 0;
  int file;
  int flag_pipe = 0;

  while(1) {
    printf("seashell:%s$ ", getcwd(cwd, sizeof(cwd)));
    fgets(input, sizeof(input), stdin);
    input[strlen(input)-1]='\0';

    //Parsing commands
    num_commands = count_commands(input);
    comm_array[0] = strtok(input, ";");
    int i = 1;
    while (i < num_commands) {
      comm_array[i] = strtok(NULL, ";");
      if(i > num_commands)
        return 0;
      i++;
    }

    for( i = 0; i < num_commands; i++) {
      char *args_array[10];
      command = comm_array[i];
      int num_args = count_args(command);

      char *comm = strtok(command, " ");
    
      if (!strcmp(comm,"exit")) {
        exit(0);
      }
    
      args_array[0] = comm;
      int j = 1;    
      if (num_args == 0) {
        args_array[1]=NULL;
      }

      else {
        while (j <= num_args) {
          args_array[j] = strtok(NULL, " ");
          if (!strcmp(args_array[j], ">")) {
            flag_redir_type = 1;
            flag_redir = j;
          }
          else if (!strcmp(args_array[j],">>")) {
            flag_redir_type = 2;
            flag_redir = j;
          }
          else if (!strcmp(args_array[j],"<")) {
            flag_redir_type = 3;
            flag_redir = j;
          }
          else if (!strcmp(args_array[j],"|")) {
	    flag_pipe = j;
	  }
          j++;
        }  
        args_array[j]=NULL;
      }

      if (!strcmp(comm,"cd")) {
        if (!args_array[1]) {
	  chdir(getenv("HOME"));
        }
        chdir(args_array[1]);
      }

      else if (flag_redir) {
        if (flag_redir_type <= 2) {
          if (flag_redir_type == 2) {
            file = open(args_array[flag_redir + 1], O_CREAT | O_WRONLY| O_APPEND, 0644);
          }
          else if (flag_redir_type == 1) {
            file = open(args_array[flag_redir + 1], O_CREAT | O_WRONLY| O_TRUNC, 0644);
          }
          pid = fork();
          if (!pid) {
            dup2(file,STDOUT_FILENO);
            int n;
            char *exec_args[10];
            for(n = 0; n < flag_redir; n++) {
              exec_args[n] = args_array[n];
            }
            exec_args[n] = NULL;
            execvp(args_array[0], exec_args);
            return 0;
          }
        }
        else if (flag_redir_type == 3){ 
          int input_file = open(args_array[flag_redir + 1], O_RDONLY, 0644);
          dup2(input_file, STDIN_FILENO);
          execvp(args_array[flag_redir - 1], NULL);
        }
        flag_redir = 0;
        flag_redir_type = 0;
      }
      
      else if (flag_pipe) {
	file = open("pipes_temp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	int pid = fork();
	if (!pid) {
	  dup2(file, STDOUT_FILENO);
	  int n;
	  char *exec_args[10];
	  for(n = 0; n < flag_pipe; n++) {
	    exec_args[n] = args_array[n];
	  }
	  exec_args[n] = NULL;
	  execvp(args_array[0], exec_args);
	  
	  //char str[10000];
	  //fgets(str, sizeof(str), file);
	  
	  int fd[2];
	  pid_t childpid;

	  pipe(fd);
	  if(childpid = fork() == -1) {
	    perror("fork");
	    exit(1);
	  }

	  if(childpid == 0) {
	    close(fd[0]);
	  }
	  else {
	    close(fd[1]);
	  }

	  /*
	  for(n = flag_pipe + 1; n < 10; n++) {
	    exec_args[n - flag_pipe - 1] = args_array[n];
	  }
	  exec_args[n] = NULL;

	  if( popen2(exec_args, &infp, &outfp) <= 0) {
	    printf("Unable to execute program.");
	    exit(1);
	  }
	  write(infp, str, sizeof(str));
		     
	  //execvp(args_array[flag_pipe + 1], exec_args);
	  //execvp("rm", &"rm pipes_temp\0");
	  */

	  exit(0);
	}	
	close(file);
      }

      else {
        pid = fork();
        if(!pid) {
          execvp(args_array[0], args_array);  
          return WEXITSTATUS(exit_val);
        }
        waitid(P_PID, pid, infop, WEXITED);
      }
    }    
  }
}

int count_commands(char input[256]) {
  int num_commands = 1;
  char *commands = input;
  while (*commands) {
    if (*commands == ';'){
      num_commands++;
    }
    commands++;
  }
  return num_commands;
}

int count_args(char *command) {
  char *p = command;
  int num_args = 0;
  while (*p){
    if (*p == ' ') {
      num_args++;
    }
    p++;
  }
  return num_args;
}
