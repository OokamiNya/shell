#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void execute();
char * parse();
void parse_and_exec();

int main(){
  while(1){
    printf("HOLLAFORADOLLA$ ");
    // execute();
   parse_and_exec();
  }
  return 0;
}

void parse_and_exec(){
  pid_t pid;
  char command[256];
  fgets(command, 256, stdin);

  command[strlen(command)-1] = 0;

  char * s1 = command;
  char * s2;
  char * a[256];
  int i = 0;

  while (s2 = strsep(&s1, ";")){
    a[i] = s2;
    i++;
  }
  command[i] = 0;

  printf("a[0] is %s", a[0]);

  int k;
 
 for(k=0; k < i; k++){
  
     char * b1 = a[k];
     char * b2;
     char * b[256];
     int j = 0;

     while (b2 = strsep(&b1, " ")){
       b[i] = s2;
       j++;
     }
  
     pid = fork();
     int status, w;

     if (pid == 0){
       execvp(b[0],b); 
       exit(0);
     }

     else if(pid < 0){
       printf("NEIN");
     }

     else{
       waitpid(pid, &status, 0);
       if(status == 0){
	 printf("child terminated successfully\n");
       }
       if(status == 1){
	 printf("child terminated unsuccessfully\n");
       }
     }
  
     printf ("fjaw\n");
 }

}
void execute(){
  pid_t pid;
  char command[256];
  fgets(command, 256, stdin);

  command[strlen(command)-1] = 0;

  char * s1 = command;
  char * s2;
  char * a[256];
  int i = 0;

  while (s2 = strsep(&s1, " ")){
    a[i] = s2;
    i++;
  }

  printf("a[0] is %s", a[0]);
  printf("a[1] is %s", a[1]);

  command[i] = 0;

  pid = fork();
  int status, w;
  
  if (pid == 0){
    execvp(a[0],a); 
    exit(0);
  }

  else if(pid < 0){
    printf("NEIN");
  }

  else{
    waitpid(pid, &status, 0);
    if(status == 0){
      printf("child terminated successfully\n");
    }
    if(status == 1){
      printf("child terminated unsuccessfully\n");
    }
  }
  
  printf ("fjaw\n");
}


/*
char * parse(){
  char command[256];
  fgets(command, 256, stdin);

  command[strlen(command)-1] = 0;

  char * s1 = command;
  char * s2;
  char * a[256];
  int i = 0;

  while (s2 = strsep(&s1, ";")){
    a[i] = s2;
    i++;
  }

  command[i] = 0;
  return a;
}
*/
