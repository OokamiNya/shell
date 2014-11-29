#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


//pipe accepts the whole line of args, 
int doPipeStuff(char* arg){
  char* split_buffer=calloc(256,sizeof(char));
  char* arg_buffer;
  arg_buffer=calloc(256,sizeof(char));
  while (split_buffer=strsep(&arg,"|")){
    int i=0;
    char** addresses= calloc(256,sizeof(char*));
    while (arg_buffer=strsep(&split_buffer," ")){
      addresses[i++]=arg_buffer;
    }
    
    int pid=fork();
    if (!pid){
      umask(0000);
      printf("ran command %s\n",addresses[0]);
      //if pipe
      int file_in = open("piped",O_RDONLY);
      dup2(file_in,STDIN_FILENO);
      int file_out = open("piped",O_WRONLY|O_CREAT|O_TRUNC);
      dup2(file_out,STDOUT_FILENO);      
      //endif pipe
      execvp(addresses[0],addresses);//check if has > or <
    }else
      wait(-1);
  }
  return 0;
}
int main(){
  char test[256] = "ls -l|grep extra|cat";
  doPipeStuff(test);
  return 0;
}
