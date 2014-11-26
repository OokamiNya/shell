#include "shell.h"

static void sighandler(int signo){
  if (signo == SIGINT){
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  while(1) {
    printprompt();    
    int i;
    char ** parsed = execute_all();
    //runs through array of commands
    for (i = 0; parsed[i]; i++){
      //printf("parsed[%d]:%s\n",i, parsed[i]);
      execute(parsed[i]);
     }
  }
  return 0;
}

//change to fix for all paths, use ~?
void cd(char* path){
  chdir(path);
}



char** execute_all(){
  char s[256];
  fgets(s, sizeof(s), stdin);
  
  char* s1 = s;
  char *sep;
  char** args = (char**)(malloc(sizeof(char*)));
  int i = 0;

  //deleting trailing newspace
  s1 = strsep(&s1, "\n");  

  //check for pipes/redirection
  
  //parsing our command
  while (sep = strsep(&s1, ";")){
    args = (char**)realloc(args, sizeof(char*)*(i+1));
    char * temp = (char *)malloc(sizeof(char)*256);
    strcpy(temp, sep);
    temp = trim(temp);
    args[i] = temp;
    i++;
  }
  args = (char**)realloc(args, sizeof(char*)*(i));
  args[i] = NULL;
  return args;
}

void printprompt() {
  char* wd;
  //getcwd(wd, sizeof(wd));
  //printf("owl:%s$ ", wd);
  printf("owl:$ ");
}

void execute(char a[256]){
  char *s1 = a;
  char *sep;
  char** arg = NULL;
  int i = 0;
  s1 = strsep(&s1, "\n");  

  //parsing our command
  while (sep = strsep(&s1, " ")){
    //printf("sep: %sh\n", sep);
    //fix spaces
    //printf("%d\n", *sep);
    //printf("TF:%d\n", sep == "\0");
    //if (strcmp(sep, " ")) {
      i++;
      arg = realloc(arg, sizeof(char*)*i);
      arg[i-1] = sep;
      //}
  }
  arg[i] = 0;
  if (strcmp(arg[0], "exit") == 0) { //if calling exit
    exit(0);
  }
  else if (strcmp(arg[0], "cd") == 0) {//if calling cd
    cd(arg[1]);
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

char * trim (char * s) {
  int l = strlen(s);
  //trailing white space -- backwards
  while(isspace(s[l - 1])) {
    l--;
  }
  //leading white space -- forward
  while(* s && isspace(* s)){
    s++;
    l--;
  }
  return strndup(s, l);
}
