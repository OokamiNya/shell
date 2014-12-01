#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
char **parse_args(char *command) {
  char *p = command;
  int num_args = 0;
    while (*p){
      if (*p == ' ') {
        num_args++;
      }
      p++;
    }
    char **args_array = (char**)malloc(sizeof(char)*64);
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
  int num_commands = 1;
  char *commands = input;
  while (*commands) {
  if (*commands == ';'){
      num_commands++;
    }
    commands++;
  }
  char **comm_array = (char**)malloc(sizeof(char)*64);
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