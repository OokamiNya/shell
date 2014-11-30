#include "shell.h"

int run_command(char* s){
  if(s==0){
    char ss[256];
    printf("@&Z ");
    fgets(ss,255,stdin);
    ss[strlen(ss)-1]=0;
    s=ss;
  }
  char *args[256];
  char *s1=s;
  char *s2;
  char *s3;
  if(s2=strsep(&s1,";")){
    int i=0;
    while(s3=strsep(&s2," ")){
      if(strcmp(s3,"")!=0){
	args[i]=s3;
	i++;
      }
    }
    args[i]=0;
    if(strcmp(args[0],"cd")==0){
      change_directory(args);
      run_command(s1);
    }else{
      int f=fork();
      if(f){
	wait(&f);
	if(s1){ //this ensures that it will not print a new line if no other command is run after it (eg: just ls)
	  printf("\n");
	}
	run_command(s1);
      }else{
	if(strcmp(args[0],"exit")==0){
	  kill(getpid(),SIGUSR1);
	}
	execvp(args[0],args);
	printf("Command not found: %s\n",args[0]);
	exit(-1);
      }
    }
  }
  return 0;
}

int change_directory(char** args){
  char path[256];
  getcwd(path,sizeof(path));
  //printf("current working dir: %s\n",path);
  int n=chdir(args[1]);
  if(n==-1){
    printf("Directory not found: %s\n",args[1]);
  }
  char path2[256];
  getcwd(path2,sizeof(path2));
  //printf("current working dir: %s\n",path2);
  return 0;
}
