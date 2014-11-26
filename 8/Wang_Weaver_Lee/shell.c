#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


void parse(char *input);


void mycd (char** commands, int numargs) { 
   char base[256];  
   sprintf(base,"%s%s","/home/students/2015/",getenv("USER")); 

   char current[256]; 
   getcwd(current, sizeof(current)); 

   int errno; 
   char path[256] = "";  

   if(numargs == 1){ 
     strcpy(path,base);

   }else{
     strcat(path, current);
     strcat(path, "/"); 
     strcat(path, commands[1]); 
   }

   
   int err = chdir(path); 
   if (err==-1) { 
     printf("%s\n", strerror(errno)); 
   } 
 } 
 

void redirect(char *input){
  int stdouttmp = dup(STDOUT_FILENO);
  int stdintmp = dup(STDIN_FILENO);
  int closeInput = 0;
  int closeOutput = 0;
  int fdin, fdout;
  if (strchr(input, '>')){
    
    char *p = strstr(input, ">");
    if (strlen(p) <= 1) {
      return;
    }
    closeOutput = 1;
    int pend = strcspn(p + 2, " \n");
    char fileoutput[64];
    strncpy(fileoutput, p + 2, pend);
    fileoutput[pend] = 0;
    umask(0);
    fdout = open(fileoutput, O_WRONLY |  O_CREAT | O_TRUNC);
    fchmod(fdout, 0644);
    dup2(fdout, STDOUT_FILENO);
    char input2[64];
    int g = p - input;
    strncpy(input2, input, g);
    input2[g] = 0;
    strcat(input2, p + pend + 2);
    strcpy(input, input2);
  }
  if (strchr(input, '<')){
    char *p = strstr(input, "<");
    if (strlen(p) <= 1) {
      return;
    }
    closeInput = 1;
    int pend = strcspn(p + 2, " \n");
    char fileoutput[64];
    strncpy(fileoutput, p + 2, pend);
    fileoutput[pend] = 0;
    //printf("%s", fileoutput);
    fdin = open(fileoutput, O_RDONLY);
    dup2(fdin, STDIN_FILENO);
    char input3[64];
    int g = p - input;
    strncpy(input3, input, g);
    input3[g] = 0;
    strcat(input3, p + pend + 2);
    strcpy(input, input3);
  }
  parse(input);
  if (closeInput){
    close(fdin);
  }
  if (closeOutput){
    close(fdout);
  }
  dup2(stdintmp, STDIN_FILENO);
  dup2(stdouttmp, STDOUT_FILENO);

}
<<<<<<< HEAD


=======
>>>>>>> 4688ce838c2e4b16b286b89f94b75331ffa8045c
void process(char *input){
  if (strchr(input, '|')){
    int numArgs;
    char **commands = (char **) calloc(64, sizeof(char *));
    char *tmp = 0;
    tmp = strtok(input, "|");
    if (tmp == 0){
      return;
    }
    do {
      commands[numArgs] = (char *) calloc(64, sizeof(char));
      strcpy(commands[numArgs], tmp);
      numArgs++;
    } while(tmp = strtok(NULL, "|"));
    commands[numArgs]=0;
  }
  else {
    redirect(input);
  }
}
<<<<<<< HEAD

void parse(char* input){
  
 
=======
void parse(char * input){
>>>>>>> 4688ce838c2e4b16b286b89f94b75331ffa8045c
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
    mycd(commands, numArgs);
    for (i= 0; i < numArgs; i++){
      free(commands[i]);
    }
    free(commands);
  }
  else if(strcmp(commands[0], "exit") == 0){
    printf("exiting\n");
    //for (i= 0; i < numArgs; i++){
    // free(commands[i]);
    //}
    //free(commands);
    exit(0);
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
      int i = execvp(commands[0], commands);
      if (i == -1){
	printf("%s\n", strerror(errno));
      }
      exit(-1);
    }
    
  }
}


int main(){
  while(1){
    errno = 0;
    char path[256];
    getcwd(path,sizeof(path));
    
    char usr[256];
    strcpy(usr,getenv("USER"));
    
    char *p = 0;
    p = strstr(path,usr);

    if(p == NULL){
      printf("%s:%s ", usr, path);
    }else{
      p += strlen(usr) + 1;
      printf("%s:~/%s$ ",usr,p);
      *p = 0;
    }

    char input[256];
    fgets(input,sizeof(input),stdin);
    char *tmp = 0;
    tmp = strtok(input, ";");
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
      process(commands[i]);

    }
    for (i= 0; i < numArgs; i++){
      free(commands[i]);
    }
    free(commands);
  }
 
}


