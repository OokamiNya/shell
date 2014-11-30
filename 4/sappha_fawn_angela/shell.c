#include "shell.h"
#include "redirect.h"

//signal handlers
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
    char ** parsed = execute_all();//array of commands
    
    for (i = 0; parsed[i]; i++){
      //printf("parsed[%d]:%s\n",i, parsed[i]);
      execute(parsed[i]);
     }

  }
  return 0;
}

char* get_id(){
  char *id = (char*)malloc(sizeof(char)*100);
  id = getlogin();
  //printf("uid: %s\n", id);
  return id;
}

//make foolproof so cannot go beyond ~ or whatnot
void cd(char* path){
  //no path given or ~ --> change directory to home directory
  if (path == '\0' || strcmp(path, "~") == 0){
    chdir(getenv("HOME"));
  }
  
  //given path
  else {
    printf("original path: %s\n", path);
    //printf("getenv('HOME'):%s\n", getenv("HOME"));
    char *sep;
    sep = strsep(&path, "~");
    //if there was a ~, we must format before chdir-ing
    if ((sep && sep[0] == '\0')) { //this means that there was a ~
      printf("path: %s\n", path);
      //printf("sep: %s\n", sep);
      char* home;
      home = getenv("HOME"); 
 
      char * final = (char *) malloc(sizeof(char)* (1 + strlen(home)+ strlen(path)));
      strcpy(final, home);
      strncat(final, path, strlen(path));
      int success = chdir(final);
      //if not successful chdir --> path not found
      if (success == -1){
	printf("owl: cd: %s: No such file or directory\n", final);
      }
    }

    //not given ~, we simply work with cwd
    else {
      
      char w[256];
      getcwd(w, sizeof(w));
      strcat(w, "/");
    
      char * final = (char *) malloc(sizeof(char)*256);
      strcpy(final, w);
      strncat(final, sep, strlen(sep));
      
      int success = chdir(final);
      if (success == -1){
      	printf("owl: cd: %s: No such file or directory\n", final);
      }
      
    }
  }  
}

char** execute_all(){
  char s[256];
  fgets(s, sizeof(s), stdin);//command-line arg
  
  char* s1 = s;
  char *sep;
  char** args = (char**)(malloc(sizeof(char*)));
  int i = 0;

  //deleting trailing newspace
  s1 = strsep(&s1, "\n");  
  
  //parsing our command by semicolons
  while (sep = strsep(&s1, ";")){
    args = (char**)realloc(args, sizeof(char*)*(i+1));
    char * temp = (char *)malloc(sizeof(char)*256);
    strcpy(temp, sep);
    temp = trim(temp);
    args[i] = temp;
    i++;
  }
  //adding terminating null
  args = (char**)realloc(args, sizeof(char*)*(i));
  args[i] = NULL;
  return args;
}


void printprompt() {
  char wd[256];
  getcwd(wd, sizeof(wd));
  //printf("get_id: %s\n", get_id());
  char *s_wd;
  s_wd = strstr(wd, get_id()) + strlen(get_id());
  printf("%s:~%s$ ", get_id(), s_wd);
}

void execute(char a[256]){
  char *s1 = a;
  char *sep;
  char** arg = (char**)(malloc(sizeof(char*)));
  int i = 0;
  s1 = strsep(&s1, "\n");  

  //first check for > , < and pipes
 
  //printf("s1: %s\n", s1);
  int has = has_redirect(s1);
  
  // printf("has:%d\n", has);
  //if redirection is necessary
  if (has){
    redirection(s1, has);
  }
  else {
    //otherwise...
    //parsing our command
    while (sep = strsep(&s1, " ")){
      //fool proofing: allows for silly things like ls  -l or other silly user input
      if (!(sep && sep[0] == '\0')) {
	arg = (char**)realloc(arg, sizeof(char*)*(i+1));
	char * tempo = (char *)malloc(sizeof(char)*256);
	strcpy(tempo, sep);
	tempo = trim(tempo);
	arg[i] = tempo;
	i++;
      }
    }
 
    //adding terminating null
    arg = (char**)realloc(arg, sizeof(char*)*(i));
    arg[i] = NULL;
    
    //if argument is 'exit' or 'quit':
    if (strcmp(arg[0], "exit") == 0 || strcmp(arg[0], "quit") == 0) { 
      exit(0);
    }
    
    //else if argument is to change directory:
    else if (strcmp(arg[0], "cd") == 0) {//if calling cd
      cd(arg[1]);
    }
    
    //otherwise, all other commands require forking:
    else { 
      int f, status;
      f = fork();
      if (f == 0) {//child process
	if (execvp(arg[0], arg) == -1){//execvp returns -1 if error returned --> aka command does not exist
	  printf("%s: command not found\n", arg[0]);
	}
      }
      else {//parent process
	wait(&status);
	//printf("status: %d\n", status);
      } 
    }
  }
  //no need to free arg: http://stackoverflow.com/questions/14492971/how-to-free-memory-created-by-malloc-after-using-execvp
  //free(arg);

}

//gets rid of trailing and leading white space 
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
