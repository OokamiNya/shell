#include "shell.h"

char** execute_all();

static void sighandler(int signo){
  if (signo == SIGINT){
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  while(1) {
    printprompt();    
    execute_all();
    //int i; 
    //for (i = 0; parsed[i]; i++){
      //printf("parsed[%d]:%s hi\n",i, parsed[i]);
      //execute(parsed[i]);
      //printf("parsed[%d]:%s hi2\n",i,parsed[i]);
    // }
  }
  return 0;
}
char** execute_all(){
  char s[256];
  //printf("sizeofs:%d\n", sizeof(s));
  fgets(s, sizeof(s), stdin);
  
  char* s1 = s;
  char *sep;
  char** args = (char**)(malloc(sizeof(char*)));
  int i = 0;

  //deleting trailing newspace
  s1 = strsep(&s1, "\n");  
  
  //parsing our command
  while (sep = strsep(&s1, ";")){
    args = (char**)realloc(args, sizeof(char*)*(i+1));
   
    args[i] = sep;
    i++;
  }
  args = (char**)realloc(args, sizeof(char*)*(i));
  args[i] = NULL;
  int c = 0;
  for (c = 0; args[c]; c++){
    printf("args[%d]:%s\n", c, args[c]);
    execute(args[c]);
  }
  return args;
}

void printprompt() {
  char* wd;
  //getcwd(wd, sizeof(wd));
  //printf("owl:%s$ ", wd);
  printf("owl:$");
}

void execute(char a[256]){
  char *s1 = a;
  char *sep;
  char** arg = NULL;
  int i = 0;
  s1 = strsep(&s1, "\n");  

  //parsing our command
  while (sep = strsep(&s1, " ")){
    i++;
    arg = realloc(arg, sizeof(char*)*i);
    arg[i-1] = sep;
  }
  arg[i] = 0;
  if (strcmp(arg[0], "exit") == 0) { //if calling exit
    exit(0);
  }
  else if (strcmp(arg[0], "cd") == 0) {//if calling cd
  
  }
  else { //otherwise, we need to fork
    int f, status;
    f = fork();
    if (f == 0) {//child process
      execvp(arg[0], arg);
    }
    else {//parent process
      wait(&status);
    } 
  }
  free(arg);
}
