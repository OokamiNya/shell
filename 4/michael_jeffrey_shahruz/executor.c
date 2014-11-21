#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Execs a function, parsing the input and running execvp
int exec_line(char *input);

void runs_command(char *scpy);

int main() {
  int status;
  char s[1024];
  char *s2;
  char *s1;
  char *scpy;
  int i;
  char *commands[1024];
  
  while(1) {
    printf("^_^: ");
    fgets(s,sizeof(s),stdin);


    scpy = malloc(1024);
    strcpy(scpy,s);

    // this part parses the input by ";" and puts each command in commands
    s1 = s;
    for(i=0;s1;i++){
      s2 = strsep(&s1,";");
      commands[i]=s2;
    }
    //

    for(i=0;scpy;i++) {
      scpy = commands[i];
      printf("Command(%d): %s\n",i,scpy);
      runs_command(scpy);
    }
    
  }
}

void runs_command(char *scpy) {
  int f =fork();
  char s[1024];
  char *first_arg;
  first_arg = strsep(&scpy," ");
    printf("scpy: %s\n",scpy);
    
    if(strcmp("exit",first_arg) == 0) {
      printf("exiting\n");
      exit(0);
    } else if (strcmp("cd",first_arg) == 0) {
      printf("cding\n");
    } else {
   if(f == 0) {
      exec_line(s);
    }
    else {
      wait(NULL);
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
