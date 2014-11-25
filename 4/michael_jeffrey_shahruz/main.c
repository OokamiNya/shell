#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Execs a function, parsing the input and running execvp
int exec_line(char *input);
void runs_command(char *scpy);
void trim(char *str);

int main() {
  int status;
  char s[1024];
  
  char *s1;
  char *s2;
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
    
    char *first_arg;
    first_arg = strsep(&scpy," ");
    
    //Exit
    if(strcmp("exit",first_arg) == 0) {
      printf("Exiting\n");
      exit(0);
    } 

    //CD
    else if (strcmp("cd",first_arg) == 0) {
      chdir(scpy);
      char direct[1024];
      getcwd(direct,sizeof(direct));
      printf("Current Directory: %s\n",direct);
    }
    
    else if(strchr(s,'>')) {
      printf("registered >\n");
    }
    
    else if(strchr(s,'<')) {
      printf("registered <\n");
    }
    
    else if(strchr(s,'|')) {
      printf("registered |\n");
    }
    
    else if(strchr(s,';')) {
      char *s1;
      char *s2;
      char *commands[1024];
      
      char *jefscpy;
      jefscpy=(char*)malloc(1024);
      strcpy(jefscpy,s);
      //lw -l ; pwd
      // this part parses the input by ";" and puts each command in commands
      s1 = s;
      for(i=0;s1;i++){
	s2 = strsep(&s1,";");
	commands[i]=s2;
      }
      
      for(i=0;jefscpy;i++) {
	jefscpy = commands[i];
	if(!jefscpy) {
	  break;
	}
	runs_command(jefscpy);
      }
    }
    
    //Execute
    else {
      int f = fork();
      if(f == 0) {
	exec_line(s);
	exit(0);
      }
      else {
	wait(&status);
      } 
    }    
  }
}

int exec_line(char *s) {
  char* string2;
  char *array[256];
  
  int i=0;
  for(;s[i]!='\n';i++){
  }
  s[i]=0;
  
  char* string1=s;
  
  for(i=0;string1;i++){
    string2 = strsep(&string1," ");
    array[i]=string2;
  }
  array[i]=0;
  execvp(array[0],array);
  return 0;
}

void runs_command(char *scpy) {
  trim(scpy);
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
      exec_line(s);
      exit(0);
    }
    else {
      wait(NULL);
    }
    
  }
}

void trim(char *str) {
  int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (str[begin] == ' ')
        begin++;

    while ((end >= begin) && str[end] == ' ')
        end--;

    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0';
}
