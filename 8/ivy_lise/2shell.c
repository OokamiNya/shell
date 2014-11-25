#include "shell.h"

//takes out leading and trailing spaces / new lines
char *strip (char *p){
  while (p[0] == ' ' || p[0] == '\n')
    p++;
  while(p[strlen(p)-1] == ' ' || p[strlen(p)-1] == '\n')
    p[strlen(p)-1] = '\0';
  return p;
}

void parse_string(char *s){
  //TODO: char**token;
  char *token = (char *)(malloc(sizeof(char *)*256));
  int alen = 1;
  s = strip(s);
  // count how many args 
  strcpy(token,s);
  while (token){
    token=strchr(token+1,' ');
    alen++;
  }
  
  char **argarray = (char **)(malloc(alen*sizeof(char *)));
  //delimiting stuff
  int i=0;
  token = strsep(&s," ");
  argarray[i] = (char*)malloc(256*sizeof(char));
  strcpy(argarray[i],token);
  while (token){
    //getting rid of empty tokens btwnXS arguments
    if (strlen(token)==0){
      alen--;
      argarray=realloc(argarray,alen*sizeof(char *));
    }
    else{
      argarray[i] = (char*)malloc(256*sizeof(char));
      argarray[i] = token;
      token = strsep(&s, " ");
      //   printf("token[%d]:___%s___",i,token);
      i++;
    }
  }
  argarray[i] = NULL;
  printf("token[%d]:%s\n\n",i,token);
  exec(argarray,i);
  free(token);
  free(argarray);
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
      //  printf("TEST\n");
      execvp(argarray[0],argarray);
    }
    else{
      int status;
      int w = wait(&status);
    }
  }
}

void shell(){
  //printf("begin.\n");
  struct passwd *p = getpwuid(getuid());
  //printf("whoa??\n");
  char * user = p->pw_name;
  
  //printf("mallocs here\n");
  char *s = (char *)(malloc(10*sizeof(char)));
  //char *token = (char *)(malloc(10*sizeof(char)));
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
  char *cmd = (char *)(malloc(10*sizeof(char)));
  while (cmd = strsep(&s,";")){
    cmd = strip(cmd);
    printf("cmd:%s  \n\n",cmd);
    parse_string(cmd);
  }

  /* // count how many args 
  token = s;
  while (token){
    token=strchr(token+1,' ');
    alen++;
  }
  
  s = strsep(&s,"\n");
  redirect(s);
  char **argarray = (char **)(malloc(alen*sizeof(char *)));
  //delimiting stuff
  int i=0;
  token = strsep(&s," ");
  argarray[i] = (char*)malloc(256*sizeof(char));
  argarray[i] = token;
  while (token){
    //getting rid of empty tokens btwnXS arguments
    if (strlen(token)==0){
      alen--;
      argarray=realloc(argarray,alen*sizeof(char *));
    }
    else{
      argarray[i] = (char*)malloc(256*sizeof(char));
      argarray[i] = token;
      token = strsep(&s, " ");
      i++;
    }
  }

  argarray[i] = NULL;
  
  exec(argarray, alen);
  //printf("done.\n");
  free(s);
  free(token);
  free(argarray);
  */
}

void redirect(char * s){
  char * tok;
  int len = 1;
  if(strchr(s, '>') || strchr(s, '<')){
    len = 2;
  }
  char ** top_arr = (char**)malloc(len*sizeof(char*));
  
  
  if(strchr(s,'>')){
    //do redir in stuff
    int fd, tmp_out, status;
    fd = open("who.txt", O_WRONLY | O_CREAT | O_TRUNC);
    tmp_out = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    int f = fork();
    //if child, execute who cmd
    //else, change stdout back
    if( !f ){
      execlp("who","who",NULL);
    } else {
      int w = wait( &status );
      dup2(tmp_out, STDOUT_FILENO);
      //printf("finished waiting. w: %d s: %d\n",w,status);
    }
  } else if(strchr(s, '<')){
    //redirin
  }
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
