#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include "parse.h"
#include "execute.h"
#include "redirect.h"

#define BUF_SIZE 256

int main(){
  char buf[BUF_SIZE];

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
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
       fprintf(stdout, "%s\n", cwd);

    printf("> ");
    fgets(buf, sizeof(buf), stdin);
    char** commands = parse(buf);
    //    printf("commands[0]:%s\n", commands[0]);
    i=0;
    while(commands[i]){
      //printf("commands[%d]:%s\n", i, commands[i]);
      if(commands[i]){
	if(*commands[i] == 0){
	  i++;
	  continue;
	}
	if(!strcmp(commands[i], "exit")){
	  printf("Exiting.\n");
	  exit(EXIT_SUCCESS);
	}
	else{
	  //add an if for cd later
	  fflush(stdout);
	  execute(commands[i]);
	}
      }
      i++;
    }

    //leave:


  }

  return 1;
}
