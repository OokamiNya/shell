#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

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
    
    // this part parses the input by ";" and puts each command in commands
    s1 = s;
    for(i=0;s1;i++){
      s2 = strsep(&s1,";");
      commands[i]=s2;
    }
    commands[i] = NULL;
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

void runs_command(char *scpy) {
  trim(scpy);
  char s[1024];
  char *first_arg;
  strcpy(s,scpy);
  
  first_arg = strsep(&scpy," ");
  //Exit
    if(strcmp("exit",first_arg) == 0 || strcmp("Exit",first_arg) == 0) {
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
      char *scpy2 = (char *)malloc(1024);
      strcpy(scpy2, s);
      char *first_cmd = (char *)malloc(1024);
      first_cmd = strsep(&scpy2, ">");
 
      char *second_cmd = (char *)malloc(1024);
      strcpy(second_cmd, scpy2);
      trim(first_cmd);
      trim(second_cmd);
      //printf("first :%s:\n", first_cmd);
      //printf("secnd :%s:\n", second_cmd);

      int f, fd, s, temp, status;
      f = fork();

      if( !f ){
	fd = open(second_cmd,O_CREAT | O_WRONLY | O_EXCL, 0644);
	temp = dup(STDOUT_FILENO);
	dup2(STDOUT_FILENO, temp);
	dup2(fd, STDOUT_FILENO);
	exec_line(first_cmd);
	close(fd);
	close(temp);
	exit(0);
      } else {
	wait(NULL);
      }

    }
    
    else if(strchr(s,'<')) {
      char *scpy2 = (char *)malloc(1024);
      strcpy(scpy2, s);
      char *first_cmd = (char *)malloc(1024);
      first_cmd = strsep(&scpy2, "<");
      
      char *second_cmd = (char *)malloc(1024);
      strcpy(second_cmd, scpy2);
      trim(first_cmd);
      trim(second_cmd);
      //printf("first :%s:\n", first_cmd);
      //printf("secnd :%s:\n", second_cmd);
      /*
      int f, fd, s, temp, status;
      f = fork();
      
      if( !f ){
	fd = open(second_cmd, O_RDONLY);
	temp = dup(STDIN_FILENO);
	dup2(STDIN_FILENO, temp);
	dup2(fd, STDIN_FILENO);
	exec_line(first_cmd);
	close(fd);
	close(temp);
	exit(0);
      } else {
	wait(NULL);
	} */ 
    }
    
    else if(strchr(s,'|')) {
      printf("registered |\n");

      char *s1;
      char *s2;
      char *commands[1024];
      char *jscpy = malloc(1024);
      int i;
      s1 = s;
      for(i=0;s1;i++){
	s2 = strsep(&s1,"|");
	trim(s2);
	commands[i]=s2;
      }
      commands[i] = NULL;

      for(i=0;jscpy;i++) {
	jscpy = commands[i];
	if(!jscpy) {
	  break;
	}
	dup2(STDOUT_FILENO, STDIN_FILENO);
	exec_line(jscpy);
      } 

      
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

int exec_line(char *s) {
  printf("exec line: %s\n",s);
  trim(s);
  char* string2;
  char *array[256];
  
  int i=0;
  
  if( strchr(s, '\n') ){
    for(;s[i]!='\n';i++){
    }
    s[i]=0;
  }
  
  char* string1=s;
  
  for(i=0;string1;i++){
    string2 = strsep(&string1," ");
    array[i]=string2;
  }
  array[i]=0;
  execvp(array[0],array);
  return 0;
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
