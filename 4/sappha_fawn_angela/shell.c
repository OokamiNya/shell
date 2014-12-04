#include "shell.h"
#include "redirect.h"

//signal handler
static void sighandler(int signo){
  if (signo == SIGINT){
    exit(0);
  }
}

//running our shell~
int main() {
  signal(SIGINT, sighandler);
  //continuously 
  while(1) {
    printprompt();    
    int i;
    char ** parsed = parser();//array of user inputed commands
  
    if (parsed != NULL){
      //printf("parsed[0]: '%s'\n", parsed[0]);
      for (i = 0; parsed[i]; i++){
	//printf("parsed[%d]:%s\n",i, parsed[i]);
	if (parsed[i] != NULL)
	  execute(parsed[i]);
      }
    }
  }
  return 0;
}

/*======== char* get_id() =======================
  Inputs:  N/A
  Returns: A string representing the currently-logged-in user's uid.
  
  A simple helper function which returns the current user's uid as a dynamically allocated string.
  ==============================================*/ 
char* get_id(){
  char *id = (char*)malloc(sizeof(char)*100);
  id = getlogin();
  //printf("uid: %s\n", id);
  return id;
}

/*================= void cd() =======================
  Inputs:  char* path
  Returns: N/A
  
  Changes our current working directory in a Linux-esque fashion:
  If not given any arguments, then change working directory to ~. Otherwise, it will change the current working directory to the given argument or yell at you.
  ==============================================*/ 
void cd(char* path){
  //no path given or ~ --> cd to home directory
  if (path == '\0' || strcmp(path, "~") == 0){
    chdir(getenv("HOME"));
  }
  //given a specific path
  else {
    char *sep;
    sep = strsep(&path, "~");
    //if there was a ~, we must format before chdir-ing
    if ((sep && sep[0] == '\0')) { //this means that there was a ~ ; must be formatted
      char* home = getenv("HOME"); 
      char * final = (char *) malloc(sizeof(char)* (1 + strlen(home)+ strlen(path)));
      strcpy(final, home);
      strncat(final, path, strlen(path));
      
      int success = chdir(final);
      
      //if not successful chdir --> path not found
      if (success == -1){
	printf("owl: cd: %s: No such file or directory\n", final);
      }
    }

    //not given ~, we simply work with cwd (ex: Documents)
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
  } //end else (given path) 
}//end cd

/*======== char** parser() =======================
  Inputs:  N/A
  Returns: An array of strings, in which each element is a command to be executed.
  
  Takes the user input and parses along the semicolons (;), adding each element into the args array of strings. Then, we return this array of strings.	
  ==============================================*/ 
char** parser(){
  char s[256];
  fgets(s, sizeof(s), stdin);//user input
  char* s1 = s;
  char *sep;
  char** args = (char**)(malloc(sizeof(char*)));
  int i = 0;
  //deleting trailing newspace
  s1 = strsep(&s1, "\n");  
  s1 = trim(s1);
  //if nothing is typed (hit enter or just spaces), then simply reprint prompt
  if ((s1 && s1[0] == '\0')){
    //printf("emptiness sucks\n");
    return NULL;
  }
  else {
    //parsing our command by semicolons
    while (sep = strsep(&s1, ";")){
      args = (char**)realloc(args, sizeof(char*)*(i+1));
      char * temp = (char *)malloc(sizeof(char)*256);
      strcpy(temp, sep);
      if (temp && (temp[0] != '\0')) { //if there isn't an empty argument
	temp = trim(temp);
	args[i] = temp;
	i++;
      }
      else {
	printf("skipped null\n");
      }
    }
    if (!i)//if i == 0, aka no inputs
      return NULL;
    else {
      //adding terminating null to properly terminate our array
      args = (char**)realloc(args, sizeof(char*)*(i));
      args[i] = NULL;
      return args;
    }
  }
}

/*======== void printprompt() =======================
  Inputs:  N/A
  Returns: N/A
  
  Prints (to stdout) our shell's prompt, which is meant to mimic Linux's bash prompt. It is the concatenation of the user's username, :, and Linux-esque pwd (~/stuff/substuff).
  For example...
  if username is 'unicorns', pwd is '/home/unicorns/Desktop', our prompt will print 'unicorns:~/Desktop'	
  ==============================================*/ 
void printprompt() {
  char wd[256];
  getcwd(wd, sizeof(wd));
  //printf("get_id: %s\n", get_id());
  char *s_wd;
  s_wd = strstr(wd, get_id()) + strlen(get_id());
  printf("%s@OWL:~%s$ ", get_id(), s_wd);
}

/*======== void execute() =======================
  Inputs:  char a[256]
  Returns: N/A
  
  Executes a single command (ls -l | wc would be considered one command in our case). First, we check for any type of redirection-esque commands (<, >, >>, |) and take care of those commands. If there are no redirection-esque commands, then we simply parse on spaces. If our command is 'cd' or 'exit' or 'quit' then we cd, exit, or quit. Otherwise, we fork and let our child process execvp and run the command. 
  ==============================================*/
int execute(char a[256]){
  char *s1 = a;
  char *sep;
  char** arg = (char**)(malloc(sizeof(char*)));
  int i = 0;
  s1 = strsep(&s1, "\n");  

  //first check for > , < and |
  int has = has_redirect(s1);
  //printf("has:%d\n", has);

  //if something like 'ls ;' 
  if (s1[0] == '\0'){
    //printf("empty arg\n");
    return 0;
  }
  //if redirection is necessary (has <, >, |)
  else if (has){
    redirection(s1, has);
  }
  
  //not a redirection cmd, parse by spaces and exec
  else {
    while (sep = strsep(&s1, " ")){
      //fool proofing: allows for silly things like ls   -l or other silly user input!
      if (!(sep && sep[0] == '\0')) {
	arg = (char**)realloc(arg, sizeof(char*)*(i+1));
	char * tempo = (char *)malloc(sizeof(char)*256);
	strcpy(tempo, sep);
	tempo = trim(tempo);
	arg[i] = tempo;
	i++;
      }
    }
 
    //adding terminating null to our array
    arg = (char**)realloc(arg, sizeof(char*)*(i));
    arg[i] = NULL;
    
    //if command is 'exit' or 'quit':
    if (strcmp(arg[0], "exit") == 0 || strcmp(arg[0], "quit") == 0) { 
      exit(0);
    }
    
    //else if command is to change directory:
    else if (strcmp(arg[0], "cd") == 0) {//if calling cd
      cd(arg[1]);
    }
    
    //otherwise, all other commands require forking:
    else { 
      int f, status;
      f = fork();
      if (f == 0) {//child process
	if (execvp(arg[0], arg) == -1){//execvp returns -1 if error returned --> aka command does not exist or other wonky error
	  //printf("errno: %d\n", errno);
	  printf("%s\n", strerror(errno));
	  //printf("%s: command not found\n", arg[0]);	
	  return -1;
	}
      }
      else {//parent process
	wait(&status);
	//printf("status: %d\n", status);
      } 
    }
  }
  return 0;
}

//gets rid of trailing and leading white space
/*======== char * trim() =======================
  Inputs:  char *s
  Returns: Pointer to the beginning of a string.
  
  Removes leading and trailing whitespace on the string s. Terminating '\0' is placed at the appropriate new location by strndup.
  ==============================================*/
char* trim (char * s) {
  int l = strlen(s);
  //trailing white space -- backwards
  while(isspace(s[l - 1])) {
    l--;
  }
  //leading white space -- forward
  while(*s && isspace(*s)){
    s++;
    l--;
  }
  return strndup(s, l);
}
