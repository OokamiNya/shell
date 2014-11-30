#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

char* removeNewLine(char*s){
  char*c;
  c = (char*)malloc(sizeof(char*));
  return strncpy(c,s,strlen(s)-1);
}

char* removeSpace(char*s){
  char* c;
  c = (char*)malloc(sizeof(char*));
  if(s[0] == ' ')
    c = &s[1];
  return c;
}

char** splitSemi(char *s){
  char** array;
  array = (char**)malloc(sizeof(char**));
  char* s3;
  s3 = (char*)malloc(sizeof(char*));
  int arrayCtr = 0;
  int ctr = 0;
  while((s3 = strsep(&s,";")) != NULL){
    array[ctr] = s3;
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
  int f = fork();
  if (f == 0){
    dup2(STDOUT_FILENO,STDIN_FILENO);
    execLine(array[1]);
    dup2(STDOUT_FILENO,STDIN_FILENO);
    execLine(array[0]);
    /*
    int fd = open("randomfile", O_CREAT|O_WRONLY);
    dup2(fd, STDOUT_FILENO);
    execLine(array[0]);
    close(fd);
    exit(-1);
    */
  }
  /*
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
  */
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

    //command prompt    
    printf("command: ");
    fgets(s,256,stdin);
    s = removeNewLine(s);
    //

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
