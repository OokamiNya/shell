#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

//just incase lmao
#include <errno.h>

int currentPID;

static void sighandler(int signo){
  if (signo == SIGINT && getpid() == 0){
    printf("caught interrupt\n");
    printf("%d\t%d\n", getpid(), currentPID);
    //kill(currentPID, 9);
    currentPID = getpid();
  }
}

int main(){
  signal(SIGINT, sighandler);
  printf("%d\n", getpid());
  
  char* args=calloc(256,sizeof(char));
  char** addresses=calloc(256,sizeof(char*));
  
  char* cwd=calloc(256,sizeof(char));  
  getcwd(cwd,256);
  int status;
  while (1){
    printf("(ﾉ◕ヮ◕)ﾉ*:・ﾟ✧");

    fgets(args,256,stdin);
    args = strsep(&args,"\n");


    char* p;
    for(p=args;*p;++p) *p= tolower(*p);
    //exit
    if (! strcmp(args,"exit"))
      exit(0);
    //cd
    else if (! strncmp(args,"cd",2)){
      execlp("cd","cd",args+3,NULL);
      chdir(args+3);
      getcwd(cwd,256);
      printf("%s\n", cwd);

    }else if(args[0]){//prevents empty lines
      if (! strstr(args,"|") &&
	  ! strstr(args,"<") &&
	  ! strstr(args,">")){//if no pipe or redirects
	int i=0,j=1,pid=fork();
	if (!pid){
	  addresses[0]=args;
	  do{
	    if (args[0]==32){
	      addresses[j++]=args+1+i;
	      args[i]=0;
	    }
	  }while(args[++i]);
	  execvp(args,addresses);
	  currentPID = getpid();
	  printf("Shell PID: %d\tCurrent Process PID: %d\n", getpid(), currentPID);
	}else{
	  wait(&status);
	  printf("2nd :Shell PID: %d\tCurrent Process PID: %d\n", getpid(), currentPID);

	}
      }//plan how code will be put together with nested fancy
      //for example:
      //ls | grep poop > swag.txt

    }

    
    
    
    
  }

  return 0;
}
