#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "execute.h"
#include "redirect.h"
#include <errno.h>
//#include "strsep.h"

#define BUF_SIZE 256

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
  char buf[BUF_SIZE]; //we need this for some reason pls don't buffer overflow
  strcpy(buf, input);
  buf[strlen(buf)] = '\0';
  char* running=malloc(sizeof(buf));
  strcpy(running, buf);
  int i = 0;
  int j = 0;
  char** args = calloc(5,BUF_SIZE); // function and args
  char* prev;
  char** args2 = calloc(3, BUF_SIZE); // redirecting i.e {">", "a.txt"}
  int redir = 0; // 1 if redirect args
  while(1){

    prev = strsep(&running, " ");
    if(!prev){
      break;
    }
    if(strcmp(prev, ">") == 0 || strcmp(prev, "<") == 0 || strcmp(prev, "|") == 0){
      redir = 1;
    }
    if (redir){
      args2[j] = prev;
      j++;
    } else {
      args[i] = prev;
      i++;
    }
    args[i] = '\0';
    args2[j] = '\0';
  }

  if(strcmp(args[0], "cd") == 0){
    if(!(args[1])){
      printf("No directory specified.\n");
    }
    else if (strcmp(args[1],"~") == 0){
      //printf("chdring to %s\n", getenv("HOME"));
      if(chdir(getenv("HOME")) == -1){
        printf("%s\n", strerror(errno));
        return -1;
      }
    } else {
      //printf("chdring to %s\n", args[1]);
      if(chdir(args[1]) == -1){
	    printf("%s\n", strerror(errno));
	return -1;
      }
    }
    return 5; //chdir'd instead.
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
    //printf("redir=%d\n", redir);
    if (redir){
      //  printf("---- Redirecting ----\n\n");
      if (strcmp(args2[0], ">") == 0){
	redir_out(args,args2[1]);
      } else if (strcmp(args2[0], "<") == 0){
	redir_in(args,args2[1]);
      } else if (strcmp(args2[0], "|") == 0){
  redir_pipe(args,args2);
      }

    }
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
