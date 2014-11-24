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
 





/*void redirect(char *input){
  if (strchr(input, '>')){
    char *p = strstr(input, "> ");
    int pend = strcspn(p + 2, " \n");
    char fileoutput[64];
    strncpy(fileoutput, p + 2, pend);
    printf("%s\n", fileoutput);
    int fdout = open(fileoutput, O_WRONLY |  O_CREAT | O_APPEND);
    dup(STDOUT_FILENO
  }

  
  else{
    parse(input);
  }
}


*/  
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
    mycd(commands, numArgs);
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
    printf("%s\n", path);
    char usr[256];
    strcpy(usr,getenv("USER"));
    printf("%s\n", usr);

   
    char *p = strstr(path,usr);
    printf("%s\n", usr);

    if(p == NULL){
     
      printf("%s:%s ", usr, path);
    }else{
      printf("ahh");

      p += strlen(usr) + 1;
      // printf("%s\n", p);
    
      printf("%s:~/%s$ ",usr,p);
    }
    *p = 0;

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
      parse(commands[i]);
    }
    for (i= 0; i < numArgs; i++){
      free(commands[i]);
    }
    free(commands);
  }
}
