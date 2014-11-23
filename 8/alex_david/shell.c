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
  shell();
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


int shell(){
  printf("_$ "); //fill _ with something (maybe cwd)
  char s[1024];
  fgets(s,sizeof(s),stdin);
  int n = 1;
  char *p = s;
  while (*p){ //now splits on ';' and runs commands in succession
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
  while (k = strsep(&p," ")){
	if (strcmp(k,"")){ //removes blanks from multiple spaces
	  params[n] = k;
	  n++;
	}  
  }
  params[n] = NULL;
  if (!strcmp(params[0],"cd")){
	int i = 1;
	cd (params [i]); // note to self ~ and / don't work
	//inputting just 'cd' causes a seg fault
  }else if (!strcmp(params[0],"exit")){
	printf("Bye!\n\n");
	exit(0);
  }else{
	int f = fork();
	if (!f){
	  execvp(params[0],params);    
	}else{
	  int status;
	  wait(&status);
	  free(params);
	}
  }
}
