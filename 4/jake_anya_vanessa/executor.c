#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

int f;
extern int cont;

//////////////////////////////////////////////////////////
//  redirection, semicolon, and piping helper functions //
//////////////////////////////////////////////////////////

void arrcpy(char** src, char** dest, int start, int end){
  int i = 0;
  while (start < end && src[start]){
    dest[i] = src[start];
    i++;
    start++;
  }
  dest[i] = 0;
}

void deletetemp(char* name){
  f = fork();
  if (!f){
    execlp("rm", "rm", name, NULL);
    exit(0);
  }
  else{
    wait(f);
    f = 0;
  }
}



/*------------------------------------------------------------
int execute()
Input: char ** arg
What it does: Takes the information typed in by the user as 
char ** arg and checks for semicolons and pipes and parses the
string accordingly and then calls execute on the new parsed 
strings individually. Also takes care of cases in which cd and 
exit are called. For any cases without cd, exit, semicolons,
or pipes it calls executef() on arg.
------------------------------------------------------------*/
int execute(char ** arg) {
  int i = 0;
  
  int pipe = 0;
  int semi = 0;
  int right = 0;
  int left = 0;
  for (i; arg[i] != 0; i++){
    if (!strcmp(arg[i], "|")){
      pipe = i;
    }
    if (!strcmp(arg[i], ";")){
      semi = i;
    }
    if (!strcmp(arg[i], ">")){
      right = i;
    }
    if (!strcmp(arg[i], "<")){
      left = i;
    }
  }
  
  //handle semicolon
  if (semi){
    char * cmd1arg[256];
    arrcpy(arg, cmd1arg, 0, semi);
    
    char * cmd2arg[256];
    arrcpy(arg, cmd2arg, semi+1, 256);
    
    int stdinhold = dup(STDIN_FILENO);
    int stdouthold = dup(STDOUT_FILENO);
    
    execute(cmd1arg);
    execute(cmd2arg);
  }
  
  //handle pipe
  else if (pipe){
    char * cmd1arg[256];
    arrcpy(arg, cmd1arg, 0, pipe);
    
    char * cmd2arg[256];
    arrcpy(arg, cmd2arg, pipe+1, 256);
    
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
    deletetemp("pipinghot");
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

/*------------------------------------------------------------
int executef()
Input: char** arg
What it does: Forks of a child process and executes arg using
execvp().
------------------------------------------------------------*/
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

char *replace_string(char *str, char *old, char *rep){

  static char buffer[256];
  char *p;

  if(!(p = strstr(str, old)))
    return str;

  strncpy(buffer, str, p-str);
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(old));

  return buffer;
}

char * my_fgets(char *s, int size, FILE * stream){
  int i = size;
  int j = 0;
  char * * result;
  char ch = (char)fgetc(stream);
  while (!(ch == '\n' || ch == '\t') && i){
    s[j] = ch;
    i--;
    j++;
    ch = (char)fgetc(stream);
  }
  s[j] = 0;
  if(ch == '\t'){
    DIR * d = opendir(".");
    int i = 0;
    char * * file;
    struct dirent * entry = readdir(d);
    while(entry){
      if(strncmp(s,entry->d_name,strlen(s))){
	strcat(*result,entry->d_name);
      }
      entry = readdir(d);
    }
  }
  printf("result(from my_fgets): %s\n",*result);
  return s;
}
