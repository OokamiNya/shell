#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void command(char* comm){
  int argcount = 0;
  int i = 0;
  char delim = ' ';
  for (i;i<strlen(comm);i++){
    if (comm[i]==delim){
      argcount++;
    }
  }
  argcount=argcount+2; //1 for command itself and 1 for NULL
  char **arguments=(char **)malloc((argcount)*sizeof(char *));
  char *temp = comm;
  i = 0;
  while (argcount){
    char *blah = strsep(&temp," ");
    arguments[i]=blah;//last index would be NULL
    i++;
    argcount--;
  }
  execvp(arguments[0],arguments);
  free(arguments);
}

void shell(){
  char currdir[500];
  getcwd(currdir,sizeof(currdir));
  printf("T-SHELL: %s ",currdir);
  char uinput[256];
  fgets(uinput,sizeof(uinput),stdin);
  char *temp = uinput;
  temp=strsep(&temp,"\n");
  if (strcmp(uinput,"exit")==0){
    printf("Bye!\n");
    exit(0);
  }
  else{    
    temp=strsep(&temp," ");
    if (strcmp(temp,"cd")==0){
      char *newdir = currdir;
      strcat(newdir,"/..");
      chdir(newdir);
    }
    printf("Command:%s \n",temp);
    int childcom = fork();
    if (childcom==0){
      command(uinput);
      exit(0);
    }
    waitpid(childcom);
    shell();
    exit(0);
  }
}

int main(){
  shell();
  return 0;
}

