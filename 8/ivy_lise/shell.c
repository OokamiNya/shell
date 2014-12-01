#include "shell.h"

//takes out leading and trailing spaces / new lines
char *strip (char *p){
  while (p[0] == ' ' || p[0] == '\n')
    p++;
  while(p[strlen(p)-1] == ' ' || p[strlen(p)-1] == '\n')
    p[strlen(p)-1] = '\0';
  return p;
}

void parse_redirect(char * s){
  char * tok;
  int len = 2;
  int i = 0, append = 0;
  char ** top_arr = (char**)malloc(len*sizeof(char*));
  strip(s);
  if(strchr(s,'>') || strstr(s,">>")){
    if(strstr(s,">>")){
      append = 1;
    }
    while(tok = strsep(&s,">")){
      tok = strip(tok);
      top_arr[i] = tok;
      i++;
    }
    //printf("L: %s R: %s\n",top_arr[0],top_arr[1]);
    
    int fd, tmp_out, status;
    if(append){
      printf("Appending.\n");
      fd = open(top_arr[1], O_WRONLY|O_CREAT|O_APPEND, 0644);
    } else {
      fd = open(top_arr[1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
    }
    tmp_out = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);

    int f = fork();
    if( !f ){
      if(strchr(top_arr[0],'|')){
	piper(top_arr[0]);
      }
      else{
	parse_string(top_arr[0]);
      }
      exit(-1);
    } else {
      int w = wait( &status );
      dup2(tmp_out, STDOUT_FILENO);
      close(fd);
      //printf("finished waiting. w: %d s: %d\n",w,status);
    }
  } else if(strchr(s, '<')){
    //redirin
    char * tok;
    int len = 2;
    int i = 0;
    char ** top_arr = (char**)malloc(len*sizeof(char*));
    strip(s);
    if(strchr(s,'<')){
      while(tok = strsep(&s,"<")){
	tok = strip(tok);
	top_arr[i] = tok;
	i++;
      }
      //printf("L: %s R: %s\n",top_arr[0],top_arr[1]);
      int fd = open(top_arr[1], O_RDONLY);
      int tmp_in, status;
      
      tmp_in = dup(STDIN_FILENO);
      dup2(fd, STDIN_FILENO);

      int f = fork();
      if( !f ){
	parse_string(top_arr[0]);
	exit(-1);
      } else {
	int w = wait( &status );
	dup2(tmp_in, STDIN_FILENO);
	close(fd);
	//printf("finished waiting. w: %d s: %d\n",w,status);
      }
    }
  }
  
  free(top_arr);
  //printf("ended\n");
}

void parse_string(char *s){
  char *token = (char *)(malloc(sizeof(char)*256));
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
      token = strsep(&s, " ");
    }
    else{
      argarray[i] = (char*)malloc(256*sizeof(char));
      argarray[i] = token;
      token = strsep(&s, " ");
      i++;
    }
  }
  argarray[i] = NULL;
  exec(argarray,i);
  free(token);
  free(argarray);
}

void exec(char ** argarray, int len){
  //cmd commands
  if (strcmp(argarray[0],"exit")==0){
    //printf("exited.\n");
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
      //printf("TEST\n");
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
  char *cmd = (char *)(malloc(10*sizeof(char)));
  while (cmd = strsep(&s,";")){
    cmd = strip(cmd);
    if (strchr(cmd, '>') || strchr(cmd, '<') || strstr(cmd, ">>")){
      parse_redirect(cmd);
    }
    else if (strchr(cmd, '|')){
      piper(cmd);
    }
   
    else {
      parse_string(cmd);
    }
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
