#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>



void parse(char *input);
void mycd(){
  printf("changed\n");
}

 
/* /\* void mycd (char** commands, int numargs) { */
/*   char base[256];  */
/*   sprintf(base,"%s%s","/home/students/2015/",getenv("USER")); */
/*   char current [256]; */
/*   getcwd (current, sizeof(current)); */
/*   printf("%s\n", base); */
/*   int errno; */
/*   char *path;  */
/*   if(numargs == 1){ */
/*     path = base; */
/*   } */
/*   /\\*else{ */
/*     strcat(current, "/"); */
/*     strcat(current, path); */
/*     printf("%s\n",current); */
/*   } */
/*   *\\/ */
/*   printf("%s\n", path); */
/*   int err = chdir(path); */
/*   // printf("%d", err); */
/*   if (err==-1) { */
/*     printf("%s\n", strerror(errno)); */
/*   } */
/*   getcwd(current, sizeof(current)); */
/*   printf("%s\n", current); */
/* } 
 */



void redirect(char *input){
  int stdouttmp = dup(STDOUT_FILENO);
  int stdintmp = dup(STDIN_FILENO);
  if (strchr(input, '>')){
    char *p = strstr(input, "> ");
    int pend = strcspn(p + 2, " \n");
    char fileoutput[64];
    strncpy(fileoutput, p + 2, pend);
    fileoutput[pend] = 0;
    int fdout = open(fileoutput, O_WRONLY |  O_CREAT | O_TRUNC);
    dup2(fdout, STDOUT_FILENO);
    char input2[64];
    int g = p - input;
    strncpy(input2, input, g);
    input2[g] = 0;
    strcat(input2, p + pend + 2);
    strcpy(input, input2);
  }
  if (strchr(input, '<')){
    char *p = strstr(input, "< ");
    int pend = strcspn(p + 2, " \n");
    char fileoutput[64];
    strncpy(fileoutput, p + 2, pend);
    fileoutput[pend] = 0;
    int fdin = open(fileoutput, O_WRONLY |  O_CREAT | O_TRUNC);
    dup2(fdin, STDIN_FILENO);
    char input3[64];
    int g = p - input;
    strncpy(input3, input, g);
    input3[g] = 0;
    strcat(input3, p + pend + 2);
    strcpy(input, input3);
  }
  parse(input);
  dup2(stdintmp, STDIN_FILENO);
  dup2(stdouttmp, STDOUT_FILENO);

}
void parse(char* input){
 
  char **commands = (char **) calloc(64, sizeof(char *));
  int i ;
  int numArgs = 0;
  char *tmp = 0;
  tmp = strtok(input, " \n");

  if (tmp == 0){
    return;
  }
  do {
    commands[numArgs] = (char *) calloc(64, sizeof(char));
    strcpy(commands[numArgs], tmp);
    numArgs++;
  } while(tmp = strtok(NULL, " \n"));
  commands[numArgs]=0;

  if(strcmp(commands[0], "cd") == 0){
    mycd(input);
    for (i= 0; i < numArgs; i++){
	free(commands[i]);
      }
      free(commands);
  }
  else if(strcmp(commands[0], "exit") == 0){
    for (i= 0; i < numArgs; i++){
	free(commands[i]);
      }
      free(commands);
    exit(-1);
  }

  
  else{
  
    int p = fork();
    if(p){
      wait();
      for (i= 0; i < numArgs; i++){
	free(commands[i]);
      }
      free(commands);
    }
    else{
      execvp(commands[0], commands);
    }
    
  }
}


int main(){
  while(1){
    char path[256];
    getcwd(path,sizeof(path));
    //printf("%s\n", path);
    char usr[256];
    strcpy(usr,getenv("USER"));

    char *p = strstr(path,usr);
    p += strlen(usr) + 1;
 
    
    printf("%s:~/%s$ ",usr,p);
    char input[256];
    fgets(input,sizeof(input),stdin);
    char *tmp = strtok(input, ";");
    char **commands = (char **) calloc(64, sizeof(char *));
    int numArgs = 0;
    do {
      commands[numArgs] = (char *) calloc(64, sizeof(char));
      strcpy(commands[numArgs], tmp);
      numArgs++;
    } while(tmp = strtok(NULL, ";"));
    commands[numArgs]=0;

    int i;
    for(i = 0; i<numArgs; i++){
      redirect(commands[i]);
    }
    for (i= 0; i < numArgs; i++){
      free(commands[i]);
    }
    free(commands);
  }
}
