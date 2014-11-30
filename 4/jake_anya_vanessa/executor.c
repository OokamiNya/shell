#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int f;
extern int cont;

int execute(char ** arg) {
  int i = 0;

  int pipe = 0;
  for (i; arg[i] != 0; i++){
    if (!strcmp(arg[i], "|")){
      pipe = i;
    }
  }

  int semi = 0;
  i = 0;
  for (i; arg[i] != 0; i++){
    if (!strcmp(arg[i], ";")){
      semi = i;
    }
  }

  //handle semicolon
  if (semi){
    char * cmd1arg[256];
    i = 0;
    while (i < semi){
      cmd1arg[i] = arg[i];
      i++;
    }

    char * cmd2arg[256];
    i = 1;
    while (arg[semi + i] != 0){
      cmd2arg[i-1] = arg[semi + i];
      i++;
    }
    
    int stdinhold = dup(STDIN_FILENO);
    int stdouthold = dup(STDOUT_FILENO);
    
    execute(cmd1arg);
    execute(cmd2arg);
  }
  
  //handle pipe
  else if (pipe){
    char * cmd1arg[256];
    i = 0;
    while (i < pipe){
      cmd1arg[i] = arg[i];
      i++;
    }
    cmd1arg[i] = 0;
    
    char * cmd2arg[256];
    i = 1;
    while (arg[pipe + i] != 0){
      cmd2arg[i-1] = arg[pipe + i];
      i++;
    }
    cmd2arg[i] = 0;
    
    int stdinhold = dup(STDIN_FILENO);
    int stdouthold = dup(STDOUT_FILENO);
    
    int fd = open("pipinghot", O_WRONLY|O_CREAT, 0644);
    dup2(fd, STDOUT_FILENO);
    execute(cmd1arg);
    dup2(stdouthold, STDOUT_FILENO);
    close(fd);

    fd = open("pipinghot", O_RDONLY, 0644);
    dup2(fd, STDIN_FILENO);
    execute(cmd2arg);
    dup2(stdinhold, STDIN_FILENO);
    close(fd);

    //delete pipinghot
    
    f = fork();
    if (!f){
      execlp("rm", "rm", "pipinghot", NULL);
      exit(0);
    }
    else{
      wait(f);
      f = 0;
    }
  }

  //handles cd
  else if(!strcmp(arg[0],"cd")){
    if (arg[1])
      chdir(arg[1]);
    else
      chdir(getenv("HOME"));
  }
  
  //handles exit
  else if(!strcmp(arg[0], "exit")){
    cont = 0;
  }
    
  else{
    executef(arg);
  }
  
  return 0;
}

int executef(char** arg){
  f = fork();
  if (!f){
    execvp(arg[0], arg);
    exit(0);
  }
  else{
    wait(f);
    f = 0;
  }
}

char *replace_string(char *str, char *orig, char *rep){

  static char buffer[4096];
  char *p;

  if(!(p = strstr(str, orig)))
    return str;

  strncpy(buffer, str, p-str);
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return buffer;
}
