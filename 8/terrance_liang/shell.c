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
  printf("argcount: %d \n", argcount);
  char **arguments=(char **)malloc((argcount)*sizeof(char *));
  char *temp = comm;
  i = 0;
  while (argcount){
    char *blah = strsep(&temp," ");
    arguments[i]=blah;//last index should be NULL
    printf("%d: %s \n",i,arguments[i]);
    i++;
    argcount--;
  }
  execvp(arguments[0],arguments);
  printf("test2\n");
  if (errno){
    printf("%s \n", strerror(errno));
  }
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
  /* code for exit */
  if (strcmp(uinput,"exit")==0){
    printf("Bye!\n");
    exit(0);
  }
  else{
    /* code for cd */
    char* newd= (char *)malloc(sizeof(temp));
    strcpy(newd,temp);
    char *cdcomm=strsep(&newd," ");
    if (strcmp(cdcomm,"cd")==0){
      char *newdir = currdir;
      if (newd){
	strcat(newdir,"/");
	strcat(newdir,newd);
      }
      else{
	strcat(newdir,"/..");
      }
      chdir(newdir);
    }
    /* code for running other commands */
    else{
      int childcom = fork();
      if (childcom==0){
	printf("Hey! I'm the child!\n");
	command(temp);
	printf("Ready to Exit\n");
	exit(childcom);
      }
      waitpid(&childcom);
    }
    shell();
    exit(0);
  }
}

int main(){
  shell();
  return 0;
}

