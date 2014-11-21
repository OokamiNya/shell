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
  printf("yo\n");
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
  char **inputA=malloc(strlen(input)*strlen(input)*sizeof(char)+sizeof(char));
  inputA=separate(input);
  printf("%s\n",inputA[0]);
  char* problemChild=inputA[0];
  /*
  if (strcmp(problemChild,"cd")){
    int i = chdir("../..");
    if (i!=0){
      printf("%s is not a directory.",problemChild);
    }
    return;
  }
  if (strcmp(problemChild,"exit")){
    printf("Thanks for using our shell!");
    exit(0);
    }*/
  execvp(inputA[0],inputA);
  
}




int main(){
  char s[1024];
  //while (1){
  printf("wedemboyz$$:");
  fgets(s,1024,stdin);
  run(s);
    //}
  return 0;
}
