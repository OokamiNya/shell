#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include "shell.h"

static void sighandler(int signo){
  if (signo == SIGUSR1){
    exit(0);
  }
}

int main(){
  signal(SIGUSR1,sighandler);
  printf("-- BALD SHELL --\n\n");
  shell();
  return 0;
}

int cd (char* s) {
  if (!strcmp(s,"/")) return chdir(s);
  if (!strcmp(s,"~")) return chdir(getenv("HOME"));
  char path[1000];
  strcpy (path, s);
  char cwd [256];
  getcwd (cwd, sizeof(cwd));
  strcat (cwd, "/");
  strcat (cwd, s);
  return chdir(cwd);
}


int shell(){
  char cwd[256];
  getcwd(cwd,sizeof(cwd));
  int r = 0;
  char *t = cwd;
  while (*t){
    if (*t == '/'){
      r++;
    }
    t++;
  }
  if (r <= 3){
    printf("%s$ ",cwd);
  }else{
    t = cwd;
    while (r > 2){
      if (*t == '/'){
		r--;
      }
      t++;
    }
    printf(".../%s$ ",t); //lists top 3 directory levels
  }
  
  char s[1024];
  fgets(s,sizeof(s),stdin);
  int n = 1;
  char *p = s;
  while (*p){ //splits on ';' and runs commands in succession
    if (*p == ';'){
      n++;
    }
    p++;
  }
  char **commands = malloc(sizeof(char *) * n);
  p = s;
  *strchr(p,'\n') = '\0';
  n = 0;
  char *k;
  while (k = strsep(&p,";")){
	commands[n] = k;
	n++;
  }
  int i = 0;
  for (; i < n; i++){
	execute(commands[i]);
  }
  free(commands);
  shell();
}

int execute(char *s){
  if (emptyString(s)){ //does not run empty commands
	return 1;
  }
  int n = 2;
  char *p = s;
  while (*p){
	if (*p == ' '){
	  n++;
	}
	p++;
  }
  char **params = malloc(sizeof(char *) * n);
  p = s;
  n = 0;
  char *k;
  //checks if | in input
  if (strchr (p, '|')){
	k = strsep(&p,"|");
	int f = fork();
	if (!f){
	  pipeCommands(k,p);
	}else{
	  int status;
	  wait(&status);
	  free(params);
	  return 1;
	}
  }
  while (k = strsep(&p," ")){
	if (strcmp(k,"")){ //removes blanks from multiple spaces
	  params[n] = k;
	  n++;
	}  
  }
  params[n] = NULL;
  if (!strcmp(params[0],"cd")){
	if (params[1]){
	  int i = 1;
	  if (cd (params [i])) 
		printf("No such directory\n");
	}else{
	  cd("~");
	}
  }else if (!strcmp(params[0],"exit")){
	printf("Bye!\n\n");
	exit(0);
  }else{
	int f = fork();
	if (!f){
	  int y = 0;
	  for (;y < n; y++){ //redirection
		if (!strcmp(params[y],">")){
		  writeTo(params,y);
		}else if (!strcmp(params[y],">>")){
		  appendTo(params,y);
		}else if (!strcmp(params[y],"<")){
		  inputFrom(params,y);
		}
	  }
	  execvp(params[0],params);    
	}else{
	  int status;
	  wait(&status);
	  free(params);
	  return 1;
	}
  }
}

int writeTo(char **params,int n){
  int fd = open(params[n+1],O_CREAT | O_TRUNC | O_WRONLY,0644);
  dup2(fd,STDOUT_FILENO);
  params[n] = NULL;
  execvp(params[0],params);
}

int appendTo(char **params,int n){
  int fd = open(params[n+1],O_CREAT | O_APPEND | O_WRONLY,0644);
  dup2(fd,STDOUT_FILENO);
  params[n] = NULL;
  execvp(params[0],params);
}

int inputFrom(char **params,int n){
  int fd = open(params[n+1],O_RDONLY);
  dup2(fd,STDIN_FILENO);
  params[n] = NULL;
  execvp(params[0],params);
}

int pipeCommands(char *left, char *right){//limited to a single pipe
  int fd[2];
  pipe(fd);//creates pipe
  int f = fork();
  if(f == -1) {
    printf("error in forking\n");
    exit(0);
  }
  if (!f){//child process
    dup2(fd[1],STDOUT_FILENO);//writes to pipe
    executePipe(left);
  }else{//parent process
    int status;
    wait(&status);
    close(fd[1]);//close write end
    dup2(fd[0],STDIN_FILENO);//reads in from pipe
    executePipe(right);
  }
}

int executePipe(char *s){
  if (emptyString(s)){ //does not run empty commands
	return 1;
  }
  int n = 2;
  char *p = s;
  while (*p){
	if (*p == ' '){
	  n++;
	}
	p++;
  }
  char **params = malloc(sizeof(char *) * n);
  n = 0;
  p = s;
  char *k;
  while (k = strsep(&p," ")){
	if (strcmp(k,"")){ //removes blanks from multiple spaces
	  params[n] = k;
	  n++;
	}  
  }
  params[n] = NULL;
  int t = execvp(params[0],params);
  if (t == -1) {
    printf("error\n");
    exit (0);
  }
}

int emptyString(char *s){
  int i = 0;
  for (; i < strlen(s);i++){
	if (s[i] != ' '){
	  return 0;
	}
  }
  return 1;
}







