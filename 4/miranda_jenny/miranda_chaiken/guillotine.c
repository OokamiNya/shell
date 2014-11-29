#include "guillotine.h"

int main(){
  printf("What would you like to run?\n");
  char * s1=malloc(200*sizeof(char));
  s1[199]=0;
  fgets(s1,199,stdin); 
  //printf("input: %s\n",s1);
  s1[strlen(s1)-1]=0;

  char * program= malloc(200 *sizeof(char));
  program = strsep(&s1," ");

  char * args[10];
  args[0]=program;
  int a=1;
  char * s2=0;
  if (s1){
    s2=strsep(&s1," ");
  }
  while(s2){
    args[a]=malloc(10*sizeof(char));
    strcpy(args[a],s2);
    s2=strsep(&s1," ");
    a++;
  }
  args[a]=0;
  execvp(program,args);
}
