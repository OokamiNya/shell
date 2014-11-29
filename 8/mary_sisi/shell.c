#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

void print_prompt();
void print_array(char** args); //for testing purposes
void parse(char ** a); //parses user input
int contains(char ** a, char * c); //helper
int execute(char ** a); //hanldes user input


int main(){

  print_prompt();
  int run = 1;
  while(run){

    char ** args; //allocate space for up to 64 strings of up to 32 characters each
    args = (char**)malloc(sizeof(char)*64);
    char ** temp = args;

    int i=0;
    while(i<32){
      args[i] = (char*)malloc(sizeof(char)*32);
      i++;
    }

    parse(args);
    execute(args);

    //freeing doesn't work, gotta fix that
    /* while(i<32){ */
    /*   free(temp[i]); */
    /*   i++; */
    /* } */
    /* free(temp); */

    print_prompt();
  }

  return 0;
}


void print_prompt(){
  char path[256];
  getcwd(path, 256);  
  
  printf("%s$ ", path);
}


void print_array(char ** args){
  int i = 0;
  while(args[i]){
    printf("args[%d]:  %s\t",i,args[i]);
    i++;
  }
}


void parse(char ** args){

  char s1[256];
  fgets(s1, sizeof(s1), stdin);
  s1[strlen(s1)-1]='\0';

  char * s = s1;
  char * temp = strsep(&s, " ");

  int i=0;
  while(temp){
    if(strcmp(temp,"") != 0){
      strcpy(args[i],temp);
      i++;   
    }
    temp = strsep(&s," ");
  }

  //termination
  args[i] = 0;
}


int contains(char ** args, char * c){
  int i=0;
  while(args[i]){
    if (strcmp(args[i], c) == 0 ){
      return i;
    }
    i++;
  }
  return -1;
}


int execute(char ** args){
  int i;
  if((i = contains(args,";")) != -1){
    printf("\nCOMMAND WITH ';' AT INDEX %d\n\n", i);

    char ** part1 = (char**)malloc(sizeof(char*) * i);
    
    int j = 0;
    while(j < i){
      part1[j] = args[j];
      j++;
    }
    
    args += (i + 1);

    execute(part1);
    execute(args);
    
  }else if((i = contains(args,"<")) != -1  ){
    printf("\nCOMMAND WITH '<' AT INDEX %d\n\n",i);
    //not functional yet

  }else if((i = contains(args,">")) != -1  ){
    printf("\nCOMMAND WITH '>' AT INDEX %d\n\n",i);
    //not functional yet

  }else if((i = contains(args,"|")) != -1  ){
    printf("COMMAND WITH '|' AT INDEX %d\n",i);
    //not functional yet

  }else if((i = contains(args,"cd")) != -1){
    chdir(args[1]);
    //'~' doesn't  work
    //and for some reason when I tested this once I had to type "exit" three times before it exited; I wasn't able to duplicate this behavior, though
  }else if((i = contains(args,"exit")) != -1){
    exit(-1);
  }else{
    int f = fork();
    int status;
    if(!f){
      print_array(args);
      execvp(args[0], args);
    }else{
      wait(&status);
    }
  }

  return 0;

}
