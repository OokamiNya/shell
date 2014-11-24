#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
//more includes

char ** separate(char *s){
  char *pos;
  int i=0;
  //printf("%s\n",s);
  if ((pos=strchr(s, '\n')) != NULL)
    *pos = '\0';
  char *s1=s;
  char **s2=(char**)(malloc(strlen(s)*strlen(s)*sizeof(char)+sizeof(char)));
  //printf("yo\n");
  //printf("%s\n",s);

  for (i=0;s[i];i++){
    s2[i] = (char *)(malloc(strlen(s)*sizeof(char)));
    //printf("s2 :%p:\n", s2[i]);

    s2[i] = strsep(&s1, " ");
    //printf("%s\n",s2[i]);
  }

  //s2[i-1]=
  //s2[i]=(char *)malloc(25*sizeof(char));
  s2[i]=NULL;
  //printf("hey\n");
  return s2;
}
void run(char *input){
  char **inputA;//=malloc(strlen(input)*strlen(input)*sizeof(char)+sizeof(char));
  inputA=separate(input);
  //char* problemChild=(char *)malloc(strlen(inputA[0])*sizeof(char));
  //problemChild=inputA[0];
  //printf("%s\n",problemChild);
  if (!strcmp(inputA[0],"cd")){
    if(inputA[1]){
      if (inputA[1][0]=='~'){
	printf("here\n");
	char a;
	char * b=(char *) malloc(sizeof(char)*strlen(inputA[1]));
	strcpy(b,inputA[1]);
	a=b[1];
	int c=0;
	while (a){
	  inputA[1][c]=a;
	  a++;
	}
	printf("%s\n",inputA[1]);
	int j=chdir(strcat(getenv("HOME"),inputA[1]));
	return;
      }
      //printf("hey\n");
      int i = chdir(inputA[1]);
      if (i!=0){
	printf("%s is not a directory.",inputA[0]);
	return;
      }
    }
      else{
	//printf("%s\n",getenv("HOME"));
	chdir(getenv("HOME"));
	return;
      }
    }
    if (!strcmp(inputA[0],"exit")){
      int z=fork();
      if (z==0){
	printf("Thanks for using our shell!\n");}
      exit(0);
      return;
    }
    //printf("heyhey\n");
    else{
      int f=fork();
      if (!f)
	execvp(inputA[0],inputA);
      return;
    }
  }




  int main(){
    char s[1024];
    int f=1;
    while(1){
      printf("wedemboyz$$:");
      fgets(s,1024,stdin);
      //f=fork();
      run(s);
      //return 0;
      wait(NULL);
    }
    return 0;
}
