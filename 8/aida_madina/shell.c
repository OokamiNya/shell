#include "shell.h"

int main() {
  char input[256];
  char *commands[10];
  char *args[10];
  char *command;
  int num_commands, num_args, i;
  int pid;
  siginfo_t *infop;
  char cwd[256];
  int flag_redir = 0;
  int exit_val = 105;
  int flag_redir_type = 0;
  int file;
  int flag_pipe = 0;
  while(1) {
    printf("seashell:%s$ ", getcwd(cwd, sizeof(cwd)));
    fgets(input, sizeof(input), stdin);
    input[strlen(input)-1]='\0';

    //Parsing commands
    num_commands = count_tokens(input, ';') + 1;
    parse(input, num_commands, commands, ";");
    for( i = 0; i < num_commands; i++) {
      //Parsing arguments
      command = commands[i];
      num_args = count_tokens(command,' ');
      parse(command, num_args, args, " ");
      int j = 1;
      while (j <= num_args) {
        if (!strcmp(args[j], ">")) {
          flag_redir_type = 1;
          flag_redir = j;
        }
        else if (!strcmp(args[j],">>")) {
          flag_redir_type = 2;
          flag_redir = j;
        }
        else if (!strcmp(args[j],"<")) {
          flag_redir_type = 3;
          flag_redir = j;
        }
        else if (!strcmp(args[j],"|")) {
          flag_pipe = j;
        }
        j++;
      }

      if (!strcmp(args[0],"exit")) {
        exit(0);
      }

      else if (!strcmp(args[0],"cd")) {
        if (!args[1]) {
           chdir(getenv("HOME"));
        }
        chdir(args[1]);
      }

      else if (flag_redir) {
        if (flag_redir_type <= 2) {
          int n;
          char *exec_args[10];
          for(n = 0; n < flag_redir; n++) {
            exec_args[n] = (args[n]);
          }
          exec_args[n] = NULL;
          redir_out(args[flag_redir + 1], exec_args, args[0], flag_redir_type);
        }
        else if (flag_redir_type == 3){ 
          pid = fork();
          if (!pid) {
            redir_in(args[flag_redir + 1], args[flag_redir - 1]);
            return WEXITSTATUS(exit_val);
          }
          waitid(P_PID, pid, infop, WEXITED);
        }
        flag_redir = 0;
        flag_redir_type = 0;
      }
      else if (flag_pipe) {
        //Pipes part is faulty! Run at your own risk
        file = open("pipes_temp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        int pid = fork();
        if (!pid) {
          dup2(file, STDOUT_FILENO);
          int n;
          char *exec_args[10];
          for(n = 0; n < flag_pipe; n++) {
            exec_args[n] = args[n];
          }
          exec_args[n] = NULL;
          execvp(args[0], exec_args);
          
          //char str[10000];
          //fgets(str, sizeof(str), file);
          
          int fd[2];
          pid_t childpid;

          pipe(fd);
          if((childpid = fork()) == -1) {
            perror("fork");
            exit(1);
          }

          if(childpid == 0) {
            close(fd[0]);
          }
          else {
            close(fd[1]);
          }

          exit(0);

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
        flag_pipe = 0;
      }
      else {
        pid = fork();
        if(!pid) {
          execvp(args[0], args);  
          return WEXITSTATUS(exit_val);
        }
        waitid(P_PID, pid, infop, WEXITED);
      }
    }    
  }
}

int count_tokens(char *line, char delim) {
  char *p = line;
  int count = 0;
  while (*p){
    if (*p == delim) {
      count++;
    }
    p++;
  }
  return count;
}
void parse(char *line, int num, char *array[], char *delim) {
  char *first = strtok(line,delim);
  array[0] = first;
  int j = 1;
  while (j <= num) {
    array[j] = strtok(NULL, delim);
    j++;
  }
  array[j] = NULL;
}
