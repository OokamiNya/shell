#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int countDelim(char *stringy, char delim){
  //helper function that counts the occurrences of a delimeter(delim) in a string(stringy)
  int argcount = 0;
  int i = 0;
  for (i;i<strlen(stringy);i++){
    if (stringy[i]==delim){
      argcount++;
    }
  }
  return argcount;
}

void execCommand(char* comm){
  //executes command(comm)
  int argcount = countDelim(comm,' ');
  argcount=argcount+2; //1 for command itself and 1 for NULL
  char **arguments=(char **)malloc((argcount)*sizeof(char *));
  char *temp = comm;
  int i = 0;
  while (argcount){
    char *blah = strsep(&temp," ");
    arguments[i]=blah;//last index should be NULL
    i++;
    argcount--;
  }
  execvp(arguments[0],arguments);
  if (errno){
    //errno 2 results in a directory/file not found error which makes sense, but this sounds better
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

char *getHomeDir(){
  //returns home directory
  char currdir[500];
  getcwd(currdir,sizeof(currdir));
  char *cdir = currdir;
  char *tempdir=(char *)malloc(sizeof(currdir));
  strcpy(tempdir,currdir);
  while(strstr(currdir,getlogin())){
    strcpy(tempdir,currdir);
    strcat(currdir,"/..");
    chdir(currdir);
    getcwd(currdir,sizeof(currdir));
  }
  return tempdir;
}

void redirectOut(char *output, char *command ){
  //redirecting out ">"
  int childcom = fork();
  if (childcom==0){
    int filedata = open(output, O_CREAT|O_TRUNC|O_WRONLY, 0644);
    dup2(filedata, STDOUT_FILENO);
    execCommand(command);
    exit(childcom);
  }
  wait();
  dup2(dup(STDOUT_FILENO),STDOUT_FILENO);
}

void redirectIn(char *input, char *command){
  //redirecting in "<"
  int childcom = fork();
  if (childcom==0){
    int filedata = open(input, O_RDONLY, 0444);
    dup2(filedata, STDIN_FILENO);
    execCommand(command);
    exit(childcom);
  }
  wait();
  dup2(dup(STDIN_FILENO),STDIN_FILENO);
}

void mypipe(char *incommand, char *outcommand){
  //piping by redirecting in and out
  redirectOut("TempFile",outcommand);
  redirectIn("TempFile",incommand);
  remove("TempFile");
}

void runCommand(char *comm){
  //executing commands including "exit" and "cd"
  char currdir[500];
  getcwd(currdir,sizeof(currdir));
  char *temp = comm;
  //code for exit
  if (strcmp(temp,"exit")==0){
    printf("Bye!\n");
    exit(0);
  }
  else{
    //code for cd
    char *newd= (char *)malloc(sizeof(temp));
    strcpy(newd,temp);
    char *cdcomm=strsep(&newd," ");
    if (strcmp(cdcomm,"cd")==0){
      char *newdir = currdir;
      //this makes it possible to return to home directory through "cd " and "cd ~"
      if (newd && strcmp(newd,"~")!=0 && strcmp(newd," ")!=0){
	strcat(newdir,"/");
	strcat(newdir,newd);
      }
      else{
	char *homedir = getHomeDir();
	strcpy(newdir,homedir);
      }
      int test= chdir(newdir);
      if (errno && test){
	printf("'cd' error: %s \n", strerror(errno));
	errno=0;
      }
    }
    //code for running other commands
    else{
      //checks for redirecting out
      if (strchr(temp,'>')){
	char *outcomm = strsep(&temp,">");
	redirectOut(temp,outcomm);
      }
      //checks for redirecting in
      else if (strchr(temp,'<')){
	char *incomm = strsep(&temp,"<");
	redirectIn(temp,incomm);
      }	
      //checks for piping
      else if (strchr(temp,'|')){
	char *inout = strsep(&temp,"|");
	mypipe(temp,inout);
      }	
      //other commands
      else{
	int childcom = fork();
	if (childcom==0){
	  execCommand(temp);
	  exit(childcom);
	}
	wait();
      }
    }
  }
}
  
void shell(){
  //shell command that is constantly rerun after a command
  char currdir[500];
  getcwd(currdir,sizeof(currdir));
  char hostname[500];
  gethostname(hostname,sizeof(hostname));
  printf("<CShell> %s@%s: %s$ ",getlogin(),hostname,currdir);
  char uinput[5000];
  fgets(uinput,sizeof(uinput),stdin);
  char *temp = uinput;
  temp=strsep(&temp,"\n");
  int numcomm=countDelim(temp,';');
  numcomm++;
  while (numcomm){
    char *blah = strsep(&temp,";");
    runCommand(blah);
    numcomm--;
  }
  shell();
  exit(0);
}

int main(){
  //main function with the intro: get it- CShell? like SeaShell? haha. okay. i tried :(
  printf("She sells CShells by the CShore ~\n");
  printf("Welcome %s!\n", getlogin());
  shell();
  return 0;
}

