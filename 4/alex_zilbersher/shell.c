#include "shell.h"

int run_command(char* s){
  if(s==0){ //asks for input if s is null, otherwise executes remaining commands
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
  if(s2=strsep(&s1,";")){ //separates the first individual command (goes through others recursively)
    int i=0;
    while(s3=strsep(&s2," ")){ //separates white space into args
      if(strcmp(s3,"")!=0){
	args[i]=s3;
	i++;
      }
    }
    args[i]=0;
    if(strcmp(args[0],"cd")==0){ //if command is cd, will skip straight to change_directory before calling the other arguments recursively
      change_directory(args);
      run_command(s1);
    }else{
      int f=fork();
      if(f){
	wait(&f); //waits for first command to finish before recursively calling any others
	if(s1){ //this ensures that it will not print a new line if no other command is run after it (eg: just ls)
	  printf("\n");
	}
	run_command(s1);
      }else{
	if(strcmp(args[0],"exit")==0){ //if a command is exit, it will send a kill message
	  kill(getpid(),SIGUSR1);
	} //otherwise it will check for redirection
	i=0;
	int r=0;
	while(args[i]){
	  if(strcmp(args[i],">")==0){
	    r=1;
	    break;
	  } else if(strcmp(args[i],"<")==0){
	    r=2;
	    break;
	  } else if(strcmp(args[i],"|")==0){
	    r=3;
	    break;
	  } i++;
	}
	if(r){ //if there is redirection, it will deal with it appropriately and exec the command
	  redirect(i,r,args);
	}
	execvp(args[0],args); //if program has not already exited, it will regularly exec the command
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
  int n=chdir(args[1]);
  if(n==-1){
    printf("Directory not found: %s\n",args[1]);
  }
  char path2[256];
  getcwd(path2,sizeof(path2));
  return 0;
}

int redirect(int i, int r, char** args){
  args[i]=0;
  char *args2[256];
  i++;
  int j=i;
  while(args[i]){
    args2[i-j]=args[i];
    i++;
  } 
  int std;
  int fd;
  int f2=fork();
  if(r==1){ //ie: if >
    std = dup(STDOUT_FILENO);
    fd = open(args2[0], O_WRONLY|O_CREAT|O_TRUNC, 0644);
    dup2(fd, STDOUT_FILENO);
    if(f2){
      wait(&f2);
      dup2(std, STDOUT_FILENO);
      close(fd);
      exit(-1);
    }else{
      execvp(args[0],args);
      printf("Command not found: %s\n",args[0]);
      exit(-1);
    }
  }
  if(r==2){ //ie: if <
    std = dup(STDIN_FILENO);
    fd = open(args2[0], O_RDONLY, 0644 );
    dup2(fd, STDIN_FILENO);
    if(f2){
      wait(&f2);
      dup2(std, STDIN_FILENO);
      close(fd);
      exit(-1);
    }else{
      execvp(args[0],args);
      printf("Command not found: %s\n",args[0]);
      exit(-1);
    }
  }
  if(r==3){ //ie: if |
    int std2;
    std = dup(STDOUT_FILENO);
    std2 = dup(STDIN_FILENO);          
    fd=open( "pzilb.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    dup2(fd, STDOUT_FILENO);
    if(f2){
      wait(&f2);
      dup2(std, STDOUT_FILENO);
      close(fd);
      fd=open( "pzilb.txt", O_RDONLY, 0644 );
      dup2(fd, STDIN_FILENO);
      int f3=fork();
      if(f3){
	wait(&f3);
	dup2(std2, STDIN_FILENO);
	close(fd);
	execlp("rm","rm","pzilb.txt",NULL);
      }else{
	execvp(args2[0],args);
	printf("Command not found: %s\n",args2[0]);
	exit(-1);
      }
    }else{
      execvp(args[0],args);
      printf("Command not found: %s\n",args[0]);
      exit(-1);
    }
  }
  return 0;
}
