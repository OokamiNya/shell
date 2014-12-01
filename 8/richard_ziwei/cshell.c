#include "heads.h"
#include "helper.c"
#include "redir.c"

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
