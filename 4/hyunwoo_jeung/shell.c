#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//function headers
char* removeNewLine(char* s);
char* removeSpace(char* s);
char** splitSemi(char* s);
void execLine(char* s);
void execStdout(char* s);
void execStdin(char* s);
void execPipe(char* s);
void execute(char* s);

char* removeNewLine(char*s){
  char*c;
  c = (char*)malloc(sizeof(char*));
  return strncpy(c,s,strlen(s)-1);
}

char* removeSpace(char *s) {
  char*str;
  str = (char*)malloc(sizeof(char*));
  str = s;
  int i;
  int start = 0;
  int end = strlen(str) - 1;
  while (str[start] == ' ')
    start++;
  while ((end >= start) && str[end] == ' ')
    end--;
  for (i = start; i <= end; i++)
    str[i - start] = str[i];
  str[i - start] = '\0';
  return str;
}

char** splitSemi(char *s){
  char** array;
  array = (char**)malloc(sizeof(char**));
  char* s3;
  s3 = (char*)malloc(sizeof(char*));
  int arrayCtr = 0;
  int ctr = 0;
  while((s3 = strsep(&s,";")) != NULL){
    array[ctr] = removeSpace(s3);
    ctr++;
  }
  array[ctr] = NULL;
  return array;
}

void execLine(char *s){
  char** array;
  array = (char**)malloc(sizeof(char**));
  char* s3;
  s3 = (char*)malloc(sizeof(char*));
  int arrayCtr = 0;
  int ctr = 0;
  while((s3 = strsep(&s," ")) != NULL){
    array[ctr] = s3;
    ctr++;
  }
  array[ctr] = NULL;
  for(ctr = 0; array[ctr]; ctr++)
    array[ctr] = removeSpace(array[ctr]);

  int f; 
  f = fork();
  if(f == 0){
    execvp(array[0],array);
  }
  else{
    int* p;
    wait(p);
  }
}

void execStdout(char* s){
  char** array;
  array = (char**)malloc(sizeof(char**));
  char* s3;
  s3 = (char*)malloc(sizeof(char*));
  int ctr = 0;
  while((s3 = strsep(&s,">")) != NULL){
    array[ctr] = s3;
    ctr++;
  }
  array[ctr] = NULL;
  ctr = 0;
  for(ctr;array[ctr];ctr++)
    array[ctr] = removeSpace(array[ctr]);

  int f;
  f = fork();
  if(f == 0){
    int fd = open(array[1], O_CREAT | O_TRUNC,0777);
    close(fd);
    fd = open(array[1], O_CREAT | O_WRONLY,0777);
    dup2(fd, STDOUT_FILENO);
    execLine(array[0]);
    close(fd);
    exit(-1);
  }
  else{
    int* p;
    wait(p);
  }
}

void execStdin(char* s){
  char** array;
  array = (char**)malloc(sizeof(char**));
  char* s3;
  s3 = (char*)malloc(sizeof(char*));
  int ctr = 0;
  while((s3 = strsep(&s,"<")) != NULL){
    array[ctr] = s3;
    ctr++;
  }
  array[ctr] = NULL;
  for(ctr = 0; array[ctr]; ctr++)
    array[ctr] = removeSpace(array[ctr]);

  int f; 
  f = fork();
  if (f == 0){
    int fd = open(array[1], O_RDONLY);
    dup2(fd, STDIN_FILENO);
    execLine(array[0]);
    close(fd);
    exit(-1);
  }
  else{
    int* p;
    wait(p);
  }
}

void execPipe(char*s){
  char** array;
  array = (char**)malloc(sizeof(char**));
  char* s3;
  s3 = (char*)malloc(sizeof(char*));
  int ctr = 0;
  while((s3 = strsep(&s,"|")) != NULL){
    array[ctr] = s3;
    ctr++;
  }
  array[ctr] = NULL;
  for(ctr = 0; array[ctr]; ctr++)
    array[ctr] = removeSpace(array[ctr]);

  int f = fork();
  if (f == 0){
    int fd = open("randomfile", O_CREAT|O_WRONLY, 0777);
    dup2(fd, STDOUT_FILENO);
    execLine(array[0]);
    close(fd);
    exit(-1);
  }
  else{
    int *p;
    wait(p);
    f = fork();
    if(f==0){
      int fd = open("randomfile", O_RDONLY);
      dup2(fd, STDIN_FILENO);
      close(fd);
      execLine(array[1]);
      execlp("rm","rm","randomfile",NULL);
      exit(-1);
    }
  }
}

void execute(char*s){
  if(strchr(s,'|'))
    execPipe(s);
  else if(strchr(s,'>'))
    execStdout(s);
  else if(strchr(s,'<'))
    execStdin(s);
  else
    execLine(s);
}

int main() {
  while(1){
    char* s;
    s = (char*)malloc(sizeof(char*));
    char** array;
    array = (char**)malloc(sizeof(char**));
    
    printf("Shell$ ");
    fgets(s,256,stdin);
    s = removeNewLine(s);

    //exit
    if(strcmp("exit",s) == 0){
      exit(0);
    }
    //

    //cd
    if(strstr(s,"cd")){
      char* cd;
      cd = (char*)malloc(sizeof(char*));
      cd = &s[3];
      chdir(cd);
    }
    //

    //executing command
    else{
      int f;
      f = fork();

      if (f == 0){  
	array = splitSemi(s);
	int i = 0;
	for(i; array[i]; i++){
	  execute(array[i]);
	}
	free(s);
	free(array);
	exit(-1);
      }
      else{
	int* p;
	wait(p);
      }
    }
  }
  return 0; 
}
