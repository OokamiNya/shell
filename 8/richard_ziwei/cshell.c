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
  if (arg[0] == 'c' && arg[1] == 'd' && (arg[2] == 0 || arg[2] == ' ')){//got lazy, didn't want to parse
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
      printf("Invalid command: %d %s\n", errno, strerror(errno));//GIVE AN ACTUAL ERRNO
      exit(-1);
    }
  }
  wait(&pid);
}

//PIPING
char pipe_it(char arg[]){
  
  //after many attempts, time to cheese it
  
  char * orig;
  char * args = &arg[0];
  int out = dup(STDOUT_FILENO);
  while (strchr(args,'|')){
    orig = strsep(&args,"|");
    orig[strlen(orig)-1]=0;
    args++;
    if (access("dummy",F_OK)!=-1){
      //multi line piping doesnt work
      char dummy1[256];
      sprintf(dummy1,"%s < dummy",orig);
      int fd = open("dummy",O_WRONLY | O_TRUNC);
      dup2(fd,STDOUT_FILENO);
      redirection(dummy1);
      close(fd);
      dup2(out,STDOUT_FILENO);
    }
    else{
      char dummy1[256];
      sprintf(dummy1,"%s > dummy",orig);
      redirection(dummy1);
    }
  }
  char dummy1[256];
  sprintf(dummy1,"%s < dummy",args);
  redirection(dummy1);
  char dummy2[16]="rm dummy";
  normal_stuff(dummy2);

      
  

  
  return 1;
}

//REDIRECTION > < >>
//WILL CALL PIPE_IT
char redirection(char arg[]){
  if (strchr(arg,'>') || strchr(arg,'<')){
    //int out = STDOUT_FILENO;
    // int in = STDIN_FILENO;
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
	else{
	  mode = O_TRUNC;
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
    //dup2(out,STDOUT_FILENO);
    //dup2(in,STDIN_FILENO);
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
