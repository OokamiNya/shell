#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "pipe.h"
#include "redirect.h"
int count_tokens(char *, char);
char **parse_args(char *);
char **parse_commands(char[]);
int array_len(char **);
void free_array(char **);
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
      //free_array(args);
    }
    //free_array(commands);
  }
}
int count_tokens(char *line, char delim) {
  char *p = line;
  int count = 0;
  while (*p) {
    if (*p == delim) {
      count++;
    }
    p++;
  }
  return count;
}
char **parse_args(char *command) {
  int num_args = count_tokens(command,' ');
    char **args_array = (char**)malloc(sizeof(char *)*num_args);
    int j = 0;
    while (j < 32) {
      args_array[j] = (char *)malloc(sizeof(char)*32);
      j++;
    }

    args_array[0] = strtok(command," ");
    int i = 1;
    while (i <= num_args) {
      args_array[i] = strtok(NULL," ");
      i++;
    }
    args_array[i] = NULL;
    return args_array;
 }

char** parse_commands(char input[256]) {
  char *commands = input;
  int num_commands = count_tokens(commands,';');
  char **comm_array = (char**)malloc(sizeof(char *)*num_commands);
  int j = 0;
  while (j < 32) {
    comm_array[j] = (char *)malloc(sizeof(char)*32);
    j++;
  }

  comm_array[0] = strtok(input, ";");
  int k = 1;
  while (k < num_commands){
    comm_array[k] = strtok(NULL, ";");
    if (k > num_commands)
      return 0;
    k++;
  }
  comm_array[k] = NULL;
  return comm_array;
}


void free_array(char **array) {
  int j = array_len(array) - 1; 
  printf("%s\n",array[j]);
  while (j >= 0) {
    printf("%d: %s\n", j, array[j]);
    free(array[j]);
    j--;
  }
  /*
  int k;
  while (array[j] && j >= 0) {
    printf("%p\n", &(array[j]));
    k = 31;
    while (array[j][k] && k >= 0) {
      printf("Address of Char %p\n", &(array[j][k]));
      free(&(array[j][k]));
      k--;
    }
    free(array[j]);
    j--;
  }
  */
  //free(array);
}

int array_len(char **array) {
  int count = 0;
  while (array[count]) {
    count++;
  }
  return count;
}




