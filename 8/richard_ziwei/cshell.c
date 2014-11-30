#include"heads.h"
#include"helper.c"

int id;

static void sighandler(int signo){
  if (getpid() != id){
    kill(getpid(),SIGINT);
  }
}



//HANDLES THE CD AND EXIT EXCEPTIONS
char dumb_exceptions(char arg[]){
  if (arg[0] == 'c' && arg[1] == 'd' && (arg[2] == 0 || arg[2] == ' ')){
    strsep(&arg," ");
    if (!arg || strcmp(arg,"~")==0){
      chdir(getenv("HOME"));
    }
    else{
      chdir(arg);
    }
    return 1;
  }
  else if (!strcmp(arg,"exit")){
    printf("BYE\n");
    exit(-1);
  }
  return 0;
}

//HANDLES THE NORMAL COMMANDS WITH ARGUEMENTS
void normal_stuff(char arg[]){
  int pid = fork();
  if (!pid){
    char *init=arg;
    char *car; int i = 0; char *argarr[256];
    while(car){
      car = strsep(&init, " ");
      argarr[i] = car;
      i++;
    }
    argarr[i] = NULL;
    if (execvp(argarr[0], argarr) < 0){
      printf("invalid command\n");//GIVE AN ACTUAL ERRNO
      exit(-1);
    }
  }
  wait(&pid);
}

//PIPING
//STILL DOESNT WORK; HOW TO PIPE
char pipe_it(char arg[]){
  int p[2];
  char *cmd1;
  char *cmd2;
  pipe(p);
  cmd2 = strchr(arg, '|') + 2;
  cmd1 = strsep(&arg, "|");
  *(cmd1 + strlen(cmd1) - 1) = NULL;
  printf("%s.%s\n", cmd1, cmd2);
  int pid = fork();
  if (!pid) {
    close(0);
    dup(p[0]);
    close(p[1]);
    close(p[0]);
    normal_stuff(cmd2);
  } else {
    wait(&pid);
    close(1);
    dup(p[1]);
    close(p[1]);
    close(p[0]);
    normal_stuff(cmd1);
    }
  return 1;
}

//REDIRECTION > < >>
//WILL CALL PIPE_IT
char redirection(char arg[]){
  if (strchr(arg,'>') || strchr(arg,'<')){
    int pid = fork();
    if (!pid){
      char * orig;
      int fd;
      if (strchr(arg,'>')){
	orig=strsep(&arg,"> ");
	int mode;
	if (arg[1] == '>'){
	  mode = O_APPEND;
	}
	strsep(&arg," ");
	fd=open(arg, O_CREAT | O_WRONLY | mode, 0644);
	dup2(fd,STDOUT_FILENO);
      }
      else{
	orig = strsep(&arg,"< ");
	strsep(&arg," ");
	fd=open(arg, O_RDONLY);
	dup2(fd,STDIN_FILENO);
      }
      close(fd);
      //normal_stuff(orig); does not work
      execlp(orig,orig,NULL);//FIX THIS TO ACCEPT ARGS
    }
    wait(&pid);
    return 1;
  }
  else if (strchr(arg, '|')){
    pipe_it(arg);
    return 1;
  }   
  return 0;  
}


int main(){
  id=getpid();
  signal(SIGINT,sighandler);
  while (1){
    char input[256];
    char direct[256];
    getcwd(direct,sizeof(direct));
    printf("%s$ ",direct);
    fgets(input,sizeof(input),stdin);
    input[strlen(input)-1]=0;
    char * ipointer = &input[0];
    char * orig = &input[0];
    while (strchr(ipointer,';')){
      orig = strsep(&ipointer,";");
      orig = strip(orig); //MALLOCS MEMORY, comment out if you don't want memory leak
      //strip is found in helper.c
      if (!dumb_exceptions(orig) && !redirection(orig)){
	normal_stuff(orig);
      }
    }
    ipointer=strip(ipointer);
    if (!dumb_exceptions(ipointer) && !redirection(ipointer)){
	normal_stuff(ipointer);
    }
  }
  return 0;
}
