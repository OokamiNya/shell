#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void execute();

int main(){
  while(1){
    printf("$ ");
    execute();
  }
  
  return 0;
}

void execute(){
  pid_t pid;
  char command[256];
  fgets(command, 256, stdin);

  command[strlen(command)-1] = 0;

  char * s1 = command;
  char * s2;
  char * a[256];
  int i = 0;

  while (s2 = strsep(&s1, " ")){
    a[i] = s2;
    i++;
  }

  command[i] = 0;

  int pid = getpid();
  //int f = fork();
  int status;

  int f = fork();
  
  if (f == 0){
    while (wait(&status) != pid){
      execvp(a[0], a);
      //exit(1);
    }
  }
  printf("PID IS: %d \n F IS: %d", getpid(), f);
  printf ("fjaw");
  exit(1); 
}
