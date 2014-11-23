#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Execs a function, parsing the input and running execvp
int exec_line(char *input);


int main() {
  int status;
  char s[1024];
  while(1) {
    printf("^_^: ");
    fgets(s,sizeof(s),stdin);
    
    int i=0;
    for(;s[i]!='\n';i++){
    }
    s[i]=0;

    char *scpy;
    scpy=(char*)malloc(1024);
    strcpy(scpy,s);
    
    char *first_arg;
    first_arg = strsep(&scpy," ");
    
    if(strcmp("exit",first_arg) == 0) {
      printf("Exiting\n");
      exit(0);
    } 
    else if (strcmp("cd",first_arg) == 0) {
      chdir(scpy);
      getcwd(first_arg,sizeof(first_arg));
      printf("Cding to \n");
    } 
    else {
      int f = fork();
      if(f == 0) {
	exec_line(s);
      }
      else {
	wait(&status);
      }
      
    }
  }
}

int exec_line(char *s) {
  char* s2;
  char *array[256];
  
  int i=0;
  for(;s[i]!='\n';i++){
  }
  s[i]=0;
  
  char* s1=s;
  
  for(i=0;s1;i++){
    s2 = strsep(&s1," ");
    array[i]=s2;
  }
  array[i]=0;
  execvp(array[0],array);
  return 0;
}
