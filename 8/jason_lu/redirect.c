#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


void redirect(char* file,char** args){
  int fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0777);
  dup2(fd,STDOUT_FILENO);
  close(fd);
  execvp(args[0],args);
}

char* getname(char** args, char* s){
  int i = 0;
  while(args[i+1]){
    if(strcmp(args[i],s) == 0){
      return args[i+1];
    }
    i++;
  }
}

char cmdarr(char** args, char* s){
  int i = 0;
  //char** temp = (char **)malloc(sizeof(char *)* );
  while(args[i]+1){
    if(strcmp(args[i],s) == 0 ){
      args[i] = args[i+1];
      args[i+1] = 0;
      return 1;  
    }
    i++;
  }
  return 0;
}

int main(){
  char** args = (char**)malloc(sizeof(char*) * 3);
  args[0] = "ls";
  //["ls", ">", "file"];
  printf("File:%s\n", getname(args,">"));
}
