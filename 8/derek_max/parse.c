#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"
#include <errno.h>

char *removespace(char *str) // remove leading space
{
  char *temp;
  while(isspace(*str)) str++;
  if(*str == 0) 
    return str;
  
  temp = str + strlen(str) - 1;
  while(temp > str && isspace(*temp)) temp--;
  *(temp+1) = 0;
  
  return str;
}

char ** parse(char * input){
  char buf[256];
  strcpy(buf, input);
  buf[strlen(buf)] = '\0';
  char* running=malloc(sizeof(buf));
  strcpy(running, buf);
  int i = 0;
  char** args = calloc(5,256);
  char* prev;
  while(1){

    prev = strsep(&running, ";");
    if(!prev){
      break;
    }
    args[i] = removespace(prev);
    i++;
  }
  args[i] = '\0';
  //printf("%s\n%s\n%s\n", args[0], args[1], args[2]);
  return args;
}
/*
int main(){
  parse("ls -l ;   touch cat   \n");
}
*/
