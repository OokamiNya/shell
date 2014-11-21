//HAHAHAHAHAHAH
#include"heads.h"


void dumb_exceptions(char arg[]){
  if (arg[0] == 'c' && arg[1] == 'd' && (arg[2] == 0 || arg[2] == ' ')){
    strsep(&arg," ");
    chdir(arg);
    if (!arg){
      chdir(getenv("HOME"));//WE WIN 10/10
    }
  }
  else if (!strcmp(arg,"exit")){
    printf("BYE\n");
    exit(-1);
  }
}

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
    execvp(argarr[0], argarr);
  }
  wait(&pid);
}

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
	//printf("I GET THIS FAR\n");
      }
      close(fd);
      //normal_stuff(orig);
      
      execlp(orig,orig,NULL);//FIX THIS TO ACCEPT ARGS
      
    }
    wait(&pid);
    return 1;
  }/*
  else if (strchr(arg,'|')){
    int pid = fork();
    if (!pid){
      ch
   */
      
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
    dumb_exceptions(input);
    if (!redirection(input)){
      normal_stuff(input);
    }
  }
  return 0;
}
