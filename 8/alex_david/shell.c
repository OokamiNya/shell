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
  printf("-- Alex L. and David B. SHELL --\n\n"); 
  shell();
  return 0;
}

int cd (char* s) {
  if (!strcmp(s,"/")) return chdir(s);
  if (!strcmp(s,"~")) return chdir("/home"); //needs to be fixed
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
    printf(".../%s$ ",t); //now lists top 3 directory levels
  }
  
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
  //checks if | in input
  if (strchr (p, '|')){
      while (k = strsep(&p,"|")){
	if (strcmp(k,"")){ //if any blanks from multiple |
	  params [n] = k;
	  n++;
	}
	else {
	  execute(params [0]);//execute only first command if there are multiple |'s
	  break;
	}
      }
      params [n] ='\0';
      
      n = n - 1;
      while (params [n]) {
	execute (params [n]); //temporary, still working on piping
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
      if (cd (params [i])) printf("No such directory\n"); // note to self ~ and / don't work
    }else{
      cd("~");
    }
    //inputting just 'cd' causes a seg fault
  }else if (!strcmp(params[0],"exit")){
    printf("Bye!\n\n");
    exit(0);
  }else{
    int f = fork();
    if (!f){
      int y = 0;
      for (;y < n; y++){ //redirection
	if (!strcmp(params[y],">")){
	  int fd = open(params[y+1],O_CREAT | O_TRUNC | O_WRONLY,0644);
	  dup2(fd,STDOUT_FILENO);
	  params[y] = NULL;
	}else if (!strcmp(params[y],">>")){
	  int fd = open(params[y+1],O_CREAT | O_APPEND | O_WRONLY,0644);
	  dup2(fd,STDOUT_FILENO);
	  params[y] = NULL;
	}else if (!strcmp(params[y],"<")){
	  int fd = open(params[y+1],O_RDONLY);
	  dup2(fd,STDIN_FILENO);
	  params[y] = NULL;
	}
      }
      execvp(params[0],params);    
    }else{
      int status;
      wait(&status);
      free(params);
    }
  }
}
