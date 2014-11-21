#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "execute.h"
#include <errno.h>
//#include "strsep.h"

/* char* parseStr(char* str, char* match){ */
/*   int loc; //where to stop */
/*   loc = strcspn(str, match); //saw this in some pico code */
/*   char* done; */
/*   //printf("str: %s\n", str); */
/*   if (loc == strlen(str)){ */
/*     return '\0'; */
/*   } */
/*   printf("loc: %d\n", loc); */
/*   printf("done before: %p\n", done); */
/*   done = *(&str + 1); */
/*   printf("done after: %p\n", done); */
/*   printf("done: %p\n", done); */
/* } */


//Windows doesn't have strsep in string.h.  Really?


int execute(char* input){
  char buf[256]; //we need this for some reason pls don't buffer overflow
  strcpy(buf, input);
  buf[strlen(buf)] = '\0';
  char* running=malloc(sizeof(buf));
  strcpy(running, buf);
  int i = 0;
  char** args = calloc(5,256); // function and args
  char* prev;
  char** args2 = calloc(2, 256); // redirecting i.e {">", "a.txt"}
  int redir = 0; // 1 if redirect args
  while(1){

    prev = strsep(&running, " ");
    if(!prev){
      break;
    }
    if(strcmp(prev, ">") == 0){
      redir = 1;
    }
    if (redir){
      args2[i] = prev;
    } else {
      args[i] = prev;
    }
    i++;
  }
  pid_t f = fork();
  int status;
  int w;
  if (f > 0){ //parent
    w = wait(&status);
    return 1;
    //exit(0);
  }
  else if (f<0){
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  else{
      execvp(args[0], args);
      printf("%s\n", strerror(errno));
      exit(EXIT_FAILURE); //only runs if execvp fails
  }
}
/*
int main(){
  execute("ls -al > a.txt");
}
*/
