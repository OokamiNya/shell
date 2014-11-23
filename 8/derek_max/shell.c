#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include "parse.h"
#include "execute.h"

int main(){
  char buf[256];
  fgets(buf, sizeof(buf), stdin);
  char** commands = parse(buf);
  //char** commands = calloc(5, 256);
  //commands is null-terminated
  //parse here into commands
  int i = 0;
  /*
  commands[0] = "ls -al";
  commands[1] = "echo hi";
  //commands[2] = ""; //still need to fix this
  commands[2] = "exit";
  commands[3] = "exit";
  */
  while(1){
    printf("help:%p\n", *(commands[i]));
    if(!(*commands[i])){
      break;
    }
    printf("> ");
    printf("commands[%d]:%s\n", i, commands[i]);
    if(commands[i]){
      char* exit_parse = strstr(commands[i], "exit");
      if(exit_parse){
	if(strlen(exit_parse) == 4){ //assuming that exit is the only command, then this shoudl work...
	  //have to add special case for no input I think...?
	  printf("Exiting.\n");
	  exit(EXIT_SUCCESS);
	}
      }
      //add an if for cd later
      fflush(stdout);
      execute(commands[i]);
    }
    printf("hi\n");
    i++;
  }
  return 1;
}
