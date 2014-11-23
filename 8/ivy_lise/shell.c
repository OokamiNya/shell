#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

//takes out leading and trailing spaces / new lines
char *strip (char *p){
  while (p[0] == ' ' || p[0] == '\n')
    p++;
  while(p[strlen(p)-1] == ' ' || p[strlen(p)-1] == '\n')
    p[strlen(p)-1] = '\0';
  return p;
}

void exec(char ** argarray, int len){
  //cmd commands
  if (strcmp(argarray[0],"exit")==0){
    printf("exit\n");
    exit(-1);
  }
  else if (strcmp(argarray[0],"cd")==0){
    if(len < 3 || strcmp(argarray[1],"~")==0){
      chdir(getenv("HOME"));
    } else if(strncmp(argarray[1],"~/",2)==0){
      char * path = strdup(argarray[1]+2);
      chdir(getenv("HOME"));
      chdir(path);
    }
    chdir(argarray[1]);
  }
  else{
    int f = fork();
    if (f < 0){
      printf("WEIRD ERROR\n");
    }
    else if(f == 0){
      execvp(argarray[0],argarray);
    }
    else{
      int status;
      int w = wait(&status);
    }
  }
}

void shell(){
  struct passwd *p = getpwuid(getuid());
  char * user = p->pw_name;
  
  char *s = (char *)(malloc(10*sizeof(char)));
  char *command = (char *)(malloc(10*sizeof(char)));
  char *token = (char *)(malloc(10*sizeof(char)));
  int alen = 1; //+1 for NULL

  //prompt
  char cwd[256];
  getcwd(cwd,sizeof(cwd));
  if(strstr(cwd,getenv("HOME"))){
    //printf("There exists HOME in pwd.\n");
    char * path = strdup(cwd+strlen(getenv("HOME")));
    printf("%s:~%s$ ",user,path);
  } else {
    printf("%s:%s$ ",user,cwd);
  }
  fgets(s,100,stdin);

  s = strip(s);
  command = s;
  // count how many args 
  token = s;
  while (token){
    token=strchr(token+1,' ');
    alen++;
  }
  
  s = strsep(&s,"\n");
  char **argarray = (char **)(malloc(alen*3*sizeof(char)));
  //delimiting stuff
  int i=0;
  token = s;
  argarray[i] = token;
  while (token = strsep(&s," ")){
    //getting rid of empty tokens btwnXS arguments
    if (strlen(token)==0){
      alen--;
      argarray=realloc(argarray,alen*3*sizeof(char));
    }
    else{
      argarray[i] = token;
      i++;
    }
  }

  argarray[i] = NULL;
  
  exec(argarray, alen);
}


static void sighandler(int signo){
  if (signo == SIGINT){
    //SIGINT is keyboard interrupt
    exit(-1);
  }
}  
  
int main(){
  while(1){
    shell();
  }
}
