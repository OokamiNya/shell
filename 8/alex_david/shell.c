#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>

static void sighandler(int signo){
  if (signo == SIGUSR1){
    exit(0);
  }
}

int main(){
  signal(SIGUSR1,sighandler);
  printf("-- _ SHELL --\n\n"); //fill _ with name or change]
  command();
  return 0;
}

int cd (char* s) {
  char path[1000];
  strcpy (path, s);
  char cwd [256];
  getcwd (cwd, sizeof(cwd));
  printf ("%s", cwd);
 
  strcat (cwd, "/");
  strcat (cwd, s);
  printf ("%s", cwd);
  
  //strcat (cwd, "/0");
  int ret = chdir(cwd);
  printf("\n%d\n", ret);
}


int command(){
  printf("_$ "); //fill _ with something (maybe cwd)
  char s[1024];
  fgets(s,sizeof(s),stdin);
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
  *strchr(p,'\n') = '\0';
  n = 0;
  char *k;
  while (k = strsep(&p," ")){
	params[n] = k;
	n++;
  }
  params[n] = NULL;
  if (!strcmp(params[0],"cd")){
    int i = 1;
    while (!strcmp(params[i],"")){//gets rid of extra "" made by extra spaces, make i start from 0 later
      i ++;
    }
    cd (params [i]); // note to self ~ and / don't work, make ; work for other commands
  }
  int f = fork();
  if (!f){
    if (!strcmp(params[0],"exit")){
      printf("Bye!\n\n");
      dup2(STDIN_FILENO,STDOUT_FILENO);
      char parent[10];
      sprintf(parent,"%d",getppid());
      execlp("kill","kill","-10",parent,NULL);
    }
    execvp(params[0],params);    
  }else{
    int status;
    wait(&status);
    command();
  }
  return 1;
}
