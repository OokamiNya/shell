#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


void allocate_array_mem(char ** buffer, int i){
  //allocate space for pointers
  printf("about to allocate the pointers");
  printf("\n");
  buffer = (char **)malloc(i * sizeof(char *));
  //allocate space for strings at the end of each of those pointers
  printf("about to allocate the strings");
  printf("\n");
  int j = 0;
  while(j < i){
    printf("inside the allocate() loop");
    printf("\n");
    buffer[j] = (char *)malloc(64 * sizeof(char));
    printf("buffer[%d]: %d\n",j,buffer[j]);
    j++;
  }
}


void print_array(char ** args){
  printf("about to print...");
  printf("\n");
  int i = 0;
  while(i<5){ //args[i]){
    printf("inside the print() loop");
    printf("\n");
    //printf("args[%d]: %s\t", i, args[i]);
    printf("\n");
    i++;
  }
  printf("\n");
}


int main(){
  char ** test;
  printf("not allocated yet!");
  printf("\n");
  allocate_array_mem(test,5);
  printf("allocated!");
  printf("\n");
  printf("\n");
  int i;
  for(i=0;i<5;i++){
    printf("args[%d]: %d\n",i,test[i]);
  }

  //print_array(test);
}
