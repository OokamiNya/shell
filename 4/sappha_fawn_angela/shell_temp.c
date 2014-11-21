#include "shell.h"

char** parser();

static void sighandler(int signo){
  if (signo == SIGINT){
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  while(1) {
    printprompt();
    char** parsed = parser();
    //printf("args:%s",parsed[1]);
   
    int i = 0;
    //for some reason execute is not getting parsed[i] correctly
    printf("parsed[i]:%s\n",parsed[i]);
    execute(parsed[i]);
    i++;
    printf("parsed[i]:%s\n",parsed[i]);
    execute(parsed[i]);
   
  }
  return 0;
}

char** parser(){
  char s[256];
  fgets(s, sizeof(s), stdin);
  char* s1 = s;
  //printf("/ns1: %s\n", s1);
  char *sep;
  char** args = NULL;
  int i = 0;
  s1 = strsep(&s1, "\n");  

  //parsing our command
  while (sep = strsep(&s1, ";")){
    i++;
    args = realloc(args, sizeof(char*)*i);
    args[i-1] = sep;
  }
  args[i] = 0;
  //printf("args[0]:%s\n", args[0]);
  //printf("args[1]:%s\n", args[1]);
  //printf("args[2]:%s\n", args[2]);
  return args;
}

void printprompt() {
  char* wd[256];
  //getcwd(wd, sizeof(wd));
  printf("owl:%s$ ", wd);
}

void execute(char * a){ //error here with * vs {}
  printf("\na:%s\n", a); //returns empty string???
  char* s1 = a;
  printf("\ns1:%s\n", s1);
  char *sep;
  char** arg = NULL;
  int i = 0;
  if (s1 != NULL){
  
    s1 = strsep(&s1, "\n");  
  
    //parsing our command
    while (sep = strsep(&s1, " ")){
      printf("sep: %s\n", sep);
      i++;
      arg = realloc(arg, sizeof(char*)*i);
      arg[i-1] = sep;
      printf("arg[i-1]: %s\n", arg[i-1]);
    }
    arg[i] = 0;
    //printf("arg[0]:%s\n", args[0]);
    if (strcmp(arg[0], "exit") == 0) { //if calling exit
      //printf("%s", args[0]);
      exit(0);
    }
    else if (strcmp(arg[0], "cd") == 0) {//if calling cd
  
    }
    else { //otherwise, we need to fork
      //printf("%s", arg[0]);
      int f, status;
      f = fork();
      if (f == 0) {//child process
	execvp(arg[0], arg);
      }
      else {//parent process
	wait(&status);
      } 
    }
    //free(arg);
  }
}
