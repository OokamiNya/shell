#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void execute();
void parse();

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

  pid = fork();
  int status, w;
  
  if (pid == 0){
    execvp(a[0],a); 
    exit(0);
  }

  else if(pid < 0){
    printf("NEIN");
  }

  else{
    waitpid(pid, &status, 0);
    if(status == 0){
      printf("child terminated successfully\n");
    }
    if(status == 1){
      printf("child terminated unsuccessfully\n");
    }
  }
  
  printf ("fjaw\n");
}

void parse(){

}
