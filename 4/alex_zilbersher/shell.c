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
  int i=0;
  int semi=0; //for ; in commands
  while(s2=strsep(&s1," ")){
    if(strcmp(s2,"")!=0){
      if(strcmp(s2,";")!=0){
	args[i]=s2;
	i++;
      }else{
	semi=1;
	break;
      }
    }
  }
  args[i]=0;
  if(semi){
    int f=fork();
    if(f){
      wait(&f);
      printf("\n");
      run_command(s1);
    }else{
      execvp(args[0],args);
    }
  }else{
    execvp(args[0],args);
  }
  return 0;
}
