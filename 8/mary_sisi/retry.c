//JUST USING THIS AS A HOLDING PLACE FOR THE EXECUTE() AND ALLOCATE_ARRAY_MEM() FUNCTIONS; will copy into shell.c once things seem to be working

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int execute(char ** a);
void allocate_array_mem(char ** a, int i);


//note: MAKE SURE EVERYTHING GETS FREED, but also THAT THINGS WHICH SHOULDN'T BE FREED DON'T GET FREED
int execute(char ** args){

  //i is to be used as a location marker in args
  //note: after i is assigned to a return statement from contains(), i==(# of elements for which memory has to be allocated), and (i+1)==(index of args whereupon the remainder of the arguments begins)
  int i;

  if((i = contains(args,";")) != -1){
    //part1 will contain the first line of command to be run
    char ** part1;
    
    //allocate space for its contents
    allocate_array_mem(part1, i);
    
    //copy over the aforementioned first line of command from args into part1
    int j = 0;
    while(j < i){
      part1[j] = args[j];
      j++;
    }

    //free the memory of args in which the copied values and the semicolon had been stored
    j=0;
    while(j<=i){
      free(args[i]);
      j++;
    }
    
    //make args point to the remainder of the array (_exclusive_ of the semicolon)
    args += (i + 1);

    //recurse first set of commands
    execute(part1);

    //recurse everything else
    execute(args);

  }

  if((i = contains(args,"<")) != -1){
    //redirects [command(s) in a] file to stdin
  }else if((i = contains(args,">")) != -1){
    //redirects stdout to a file
  }else if((i = contains(args,"|")) != -1){
    //output of 1st to input of 2nd
  }else if((i = contains(args,"cd")) != -1){
    //covered
  }else if((i = contains(args,"exit")) != -1){
    //covered
  }else{

    //fork and exec
    int f = fork();
    int status;
    if(!f){
      execvp(args[0], args);
    }else{
      //the parent will wait until the child has finished running
      wait(&status);
    }

  }

}


//allocates memory for a string of 64 characters for each element in the array, the number of elements being indicated by i
void allocate_array_mem(char ** buffer, int i){

  while(i > 0){
    buffer[i] = (char *)malloc(64, sizeof(char));
    i--;
  }

}
