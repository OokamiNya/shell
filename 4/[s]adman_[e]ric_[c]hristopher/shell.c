#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//just incase lmao
#include <errno.h>



int main(){
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
      execlp("cd","cd",args+3);
      chdir(args+3);
      getcwd(cwd,256);
    }else if(args[0]){//if they spam enter obnoxiously like eric does -eric
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
      }else
	wait(&status);
      //pipe -> redirect
      //ls | grep poop > swag.txt

    }

    
    
    
    
  }

  return 0;
}
