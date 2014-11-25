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
    arguments[i]=blah;//last index should be NULL
    i++;
    argcount--;
  }
  execvp(arguments[0],arguments);
  if (errno){
    if (errno==2){
      printf("command error: Command not found\n");
    }
    else{
      printf("command error:%d  %s \n", errno, strerror(errno));
    }
    errno=0;
  }
  free(arguments);
}

char * getHomeDir(){
  char currdir[500];
  getcwd(currdir,sizeof(currdir));
  char * cdir = currdir;
  char * tempdir=(char *)malloc(sizeof(currdir));
  strcpy(tempdir,currdir);
  while(strstr(currdir,getlogin())){
    strcpy(tempdir,currdir);
    strcat(currdir,"/..");
    chdir(currdir);
    getcwd(currdir,sizeof(currdir));
  }
  return tempdir;
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
      if (newd && strcmp(newd,"~")!=0){
	strcat(newdir,"/");
	strcat(newdir,newd);
      }
      else{
	char* homedir = getHomeDir();
	strcpy(newdir,homedir);
      }
      chdir(newdir);
      if (errno){
	printf("'cd' error: %s \n", strerror(errno));
	errno=0;
      }
    }
    /* code for running other commands */
    else{
      int childcom = fork();
      if (childcom==0){
	command(temp);
	exit(childcom);
      }
      wait();
    }
    shell();
    exit(0);
  }
}

int main(){
  printf("Welcome %s!\n", getlogin());
  shell();
  return 0;
}

