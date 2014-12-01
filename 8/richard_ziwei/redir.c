#include "heads.h"
char pipe_it(char arg[]){
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
