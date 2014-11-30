#include "redirect.h"

//Only works for > and >> 
void redirect(char* file,char** args, char* r){
  int f = fork();
  if(!f){
      int fd = 0;
      if(strcmp(r, ">>") == 0){
        fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0777);
      }else if (strcmp(r, ">") == 0){
        fd = open(file, O_CREAT | O_WRONLY , 0777);
      }
      if(strcmp(r, "<") == 0){
        fd = open(file, O_RDONLY);
        dup2(STDIN_FILENO, fd);
        close(fd);
        execvp(args[0],args);
      }else{
        dup2(fd,STDOUT_FILENO);
        close(fd);
        execvp(args[0],args);
      }
  }
}

int printargs(char** args){
    int i = 0;
    printf("[");
    while(args[i+1]){
        printf("%s,",args[i]);
        i++;
    }
    printf("%s]",args[i]);
    return 0;
}

char* getname(char** args, char* s){
  int i = 0;
  while(args[i+1]){
    if(strcmp(args[i],s) == 0){
      return args[i+1];
    }
    i++;
  }
}

char cmdarr(char** args, char* s){
  int i = 0;
  //char** temp = (char **)malloc(sizeof(char *)* );
  while(args[i]+1){
    if(strcmp(args[i],s) == 0 ){
      args[i] = 0;
      return 1;  
    }
    i++;
  }
  return 0;
}

char* check(char** args){
    int i = 0;
    while(args[i]){
        if(strcmp(args[i], ">") == 0 || 
           strcmp(args[i], ">>") == 0 || 
           strcmp(args[i], "<") == 0 || 
           strcmp(args[i], "<<") == 0){
            return args[i];
        }
        i++;
    }
    return 0;
}
/*
int main(){
  char** args = (char**)malloc(sizeof(char*) * 5);
  args[0] = "ls";
  args[1] = "-l";
  args[2] = "<";
  args[3] = "file.txt";
  //["ls", ">", "file"];
  if(check(args)){
    printf("Thing:%s\n", check(args));
  }
  printf("File:%s\n", getname(args,"<"));
  printargs(args);
  char* filename = getname(args,"<");
  cmdarr(args,">");
  printargs(args);
  redirect(filename, args, "<");
}
*/