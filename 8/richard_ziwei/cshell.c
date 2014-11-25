#include"heads.h"

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
      printf("invalid command");
    } else execvp(argarr[0], argarr);
  }
  wait(&pid);
}

//PIPING
char pipe_it(char arg[]){
  int in = STDIN_FILENO;
  dup2(STDIN_FILENO,STDOUT_FILENO);
  dup2(STDOUT_FILENO,in);
  while(strchr(arg,'|')){
    char * orig;
    orig = strsep(&arg,"|");
    orig[strlen(orig)-1]=0;
    arg++;
    printf("orig:<%s>\targ:<%s>\n",orig,arg);
    normal_stuff(orig);
  }
  arg++;
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
  //should probably factor this so ; works
  while (1){
    char input[256];
    char direct[256];
    getcwd(direct,sizeof(direct));
    printf("%s$ ",direct);
    fgets(input,sizeof(input),stdin);
    input[strlen(input)-1]=0;
    //im sorry
    if (!dumb_exceptions(input) && !redirection(input)){
      normal_stuff(input);
    }
  }
  return 0;
}
