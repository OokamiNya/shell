#include"heads.h"
#include"helper.c"

//DOES NOT WORK
static void sighandler(int signo){
  int id = getpid();
  int pid = fork();
  if (!pid){
    printf("Intercepting...\n");

    main();
    //exit(-1);
  }
  waitpid(pid);
  printf("what");
  sleep(100000);
  exit(-1);
}



//HANDLES THE CD AND EXIT EXCEPTIONS
char dumb_exceptions(char arg[]){
  if (arg[0] == 'c' && arg[1] == 'd' && (arg[2] == 0 || arg[2] == ' ')){
    strsep(&arg," ");
    if (!arg || strcmp(arg,"~")==0){
      chdir(getenv("HOME"));//WE WIN 10/10
    }
    //else if (!strcmp(arg,".")){
    //}
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
  //printf("I WORK :D\n");
  wait(&pid);
}

//PIPING
//STILL DOESNT WORK; HOW TO PIPE
char pipe_it(char arg[]){
  int pipefd[2];
  pipe(pipefd);
  int out = STDOUT_FILENO;
  dup2(pipefd[1],STDOUT_FILENO);

  while(strchr(arg,'|')){
    
    char * orig;
    orig = strsep(&arg,"|");
    orig[strlen(orig)-1]=0;
    arg++;
    //printf("orig:<%s>\targ:<%s>\n",orig,arg);
    normal_stuff(orig);
  }
  arg++;
  close(pipefd[1]);
  dup2(out,STDOUT_FILENO);
  dup2(pipefd[0],STDIN_FILENO);
  normal_stuff(arg);
  return 1;
}

//REDIRECTION > < >>
//WILL CALL PIPE_IT
char redirection(char arg[]){
  //Multiple pipes and redirects?
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
  else if (strchr(arg,'|')){
    return pipe_it(arg);
  }   
  return 0;  
}


int main(){
 
  signal(SIGINT,sighandler);
  //semicolon now works.
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
