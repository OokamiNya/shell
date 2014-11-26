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
  char *commands[1024];
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
    
     // this part parses the input by ";" and puts each command in commands
    s1 = s;
    for(i=0;s1;i++){
      s2 = strsep(&s1,";");
      commands[i]=s2;
    }
    //

    for(i=0;scpy;i++) {
      scpy = commands[i];
      if(!scpy) {
	break;
      }
      runs_command(scpy);
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

void runs_command(char *scpy) {
  char s[1024];
  char *first_arg;
  strcpy(s,scpy);
  
  first_arg = strsep(&scpy," ");
    
  if(strcmp("exit",first_arg) == 0) {
      printf("Exiting\n");
      exit(0);
    } 
    else if (strcmp("cd",first_arg) == 0) {
      chdir(scpy);
      char direct[1024];
      getcwd(direct,sizeof(direct));
      printf("Current Directory: %s\n",direct);
    } 
    else {
      
      int f = fork();
      if(f == 0) {
	printf("execing : %s\n",s);
	exec_line(s);
       	exit(-1);
      }
      else {
	wait(NULL);
      }
      
    }

}
