#include "redirect.h"

/*======== void redirect_in() =======================
  Inputs:  char *command, char* file
  Returns: N/A
  
  Redirects stdin to a given file and executes the given command.
  Ex: cat < DESIGN.txt will redirect stdin to DESIGN.txt and then cat, resulting in cat DESIGN.txt.
  ==============================================*/ 
void redirect_in(char * command, char * file){
	int fd, fd1;
	fd = open(file ,O_RDONLY, 0644);
     	if (fd == -1){
	  printf("owl: %s: No such file or directory\n", file);
	}
	else {
	  fd1 = dup(STDIN_FILENO);//set fd1 to STDIN
	  dup2(fd, STDIN_FILENO);//set STDIN to fd
	  execute(command);
	  dup2(fd1, STDIN_FILENO);//reset STDIN to fd1
	}
}

/*======== void redirect_out() =======================
  Inputs:  char *command, char* file, int mode
  Returns: N/A
  
  Redirects stdout from executing the given command to the given file. 
  --> If mode == 1, we will create (if necessary) or rewrite the file if it already exists.
  --> If mode == 2, we will create (if necessary) or append to the file if it already exists.
  ==============================================*/
void redirect_out(char * command, char * file, int mode){
  int fd, fd1;
  if (mode == 1){
    fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  }
  else if (mode == 2){
    fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
  }
  fd1 = dup(STDOUT_FILENO);//set fd1 to stdout
  dup2(fd, STDOUT_FILENO);//redirect fd to stdout
  execute(command);
  dup2(fd1, STDOUT_FILENO);//reseting stdout: redirect fd1 to stdout
}

/*======== void redirect_err() =======================
  Inputs:  char *command, char* file, int mode
  Returns: N/A
  ***************CURRENTLY BROKEN**********************
  Redirects stderr from executing the given command to the given file. 
  --> If mode == 1, we will create (if necessary) or rewrite the file if it already exists.
  --> If mode == 2, we will create (if necessary) or append to the file if it already exists.
  ==============================================*/ 
void redirect_err(char * command, char * file, int mode){
  int fd, fd1;
  if (mode == 1){ //2>
    fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  }
  else if (mode == 2){ //2>>
    fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
  }
  fd1 = dup(STDERR_FILENO);//set fd1 to stderr
  dup2(fd, STDERR_FILENO);//redirect fd to stderr
  dup2(fd1, STDERR_FILENO);//reset: redirect fd1 to stderr
}

/*======== void pipeify() =======================
  Inputs:  char *first, char* second
  Returns: N/A
  
  Redirects the stdout from executing first to the stdin from executing second by redirecting stdout of the first command to a temporary file, then redirecting stdin of the second command to this temporary file. After this, we delete the temporary file--who said temp ever existed?!
  ==============================================*/
void pipeify(char * first, char * second) {
  //ls | wc --> ls is first, wc is second
  first = trim(first);
  second = trim(second);
  int stdin_fd, stdout_fd, fd;

  //for resetting purposes
  stdin_fd = dup(STDIN_FILENO);
  stdout_fd = dup(STDOUT_FILENO);

  fd = open("temp", O_CREAT | O_TRUNC | O_WRONLY, 0644);
  //redirecting stdout to temp
  dup2(fd, STDOUT_FILENO); //redirects fd to stdout
  execute(first);
  close(fd);
  dup2(stdout_fd, STDOUT_FILENO);//reseting stdout

  //redirecting temp to stdin
  fd = open("temp", O_RDONLY, 0644);
  dup2(fd, STDIN_FILENO); //redirecting fd to stdin
  execute(second);
  dup2(stdin_fd, STDIN_FILENO);//reset stdin

  int status, error, f;
  f = fork();
  if (f){//parent process
    //printf("parent me!\n");
    wait(&status);
  }
  else { //child process
    //printf("child me!\n");
    error = execlp("rm", "rm", "temp", NULL);
  }
 
}

/*======== void redirection() =======================
  Inputs:  char *s, int mode
  Returns: N/A
  
  The grandpa redirection function. Takes care of whatever redirection business is necessary based on mode, which, in our code, is given the value returned by has_redirect().
  ==============================================*/ 
void redirection(char *s, int mode){
  char *sep;
  char* in = (char*)malloc((sizeof(char)*256));
  strcpy(in, s);
  in = trim(in);

  if (mode == 1){// <
    sep = strsep(&in, "<");
    sep = trim(sep);
    if (in == 0) {//if null; for example ls < 
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      redirect_in(sep, in);
    }
  }
  
  else if (mode == 3){ // >
   
    sep = strsep(&in, ">");
    sep = trim(sep);
    if (in == 0) {//if null; for example ls >
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      redirect_out(sep, in, 1);//1 means trunc mode
    }
    
  }  
  else if (mode == 4){ // >>
    sep = strsep(&in, ">");
    strsep(&in, ">");
    if (in == 0) {//if null; for example ls >> 
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      //in is file
      redirect_out(sep, in, 2);//2 means append mode
    }
  }
  else if (mode == 5) { // |
    sep = strsep(&in, "|");
    sep = trim(sep);
    if (in == 0) { // if there isn't a 2nd command given
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      pipeify(sep, in);
    }
  }
  /* 2> and 2>> failed attempts:
  else if (mode == 6){ //2>
    char* temp = (char*)malloc(sizeof(char)*256);
    strcpy(temp, in);
    //printf("in: %s\n", in);
    char* cmd = strsep(&temp, "2");
    cmd = trim(cmd);
    strsep(&temp, ">");
    temp = trim(temp);
    //printf("temp: %s\n", temp);
    // printf("cmd: %s\n", cmd);
    if (temp == 0) {//if null; for example ls < 
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      redirect_err(cmd, temp, 1);
    }
  }
 
  else if (mode == 7){
  char* temp = (char*)malloc(sizeof(char)*256);
  strcpy(temp, in);
  //printf("in: %s\n", in);
    char* cmd = strsep(&temp, "2");
    cmd = trim(cmd);
    strsep(&temp, ">");
    temp = trim(temp);
    //printf("temp: %s\n", temp);
    // printf("cmd: %s\n", cmd);
    if (temp == 0) {//if null; for example ls < 
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      redirect_err(cmd, temp, 2);
      }
    }
  */
}


/*======== int has_redirect() =======================	
  Inputs:  char *i
  Returns: An integer value that gives insight as to whether or not the inputted string (i) originally had any fancy redirection symbols.
	A helper function which manipulates the inputted string i (used as a fxn in other contexts) to determine which redirection symbols exist.

	Return Value	Symbol Existent
	--------------------------------
	0		N/A
	1		<
	3		>
	4		>>
	5		|
	6		2>
	7		2>>

	Note: 6 and 7 represent attempts to redirect stderr
	Note: 2 is missing because skipping << seemed sad.
	==============================================*/ 
int has_redirect(char* i){
  char *input = (char*) malloc((sizeof(char)*256));
  strcpy(input, i);
  char *less, *more, *pipe;
  less = strchr(input, '<');
  //printf("less: %d\n", less);
  more = strchr(input, '>');
  //printf("more: %d\n", more);
  pipe = strchr(input, '|');
  if (less) { //if there is a < sign
    return 1;
  }
  else if (more) { //if there is a > sign
    char *err_check = (char*) malloc((sizeof(char)*256));
    strcpy(err_check, i);				
    //printf("err_check is %s\n", err_check);
    char *err = strstr(err_check, "2>");
    if (err) { //if has 2>
      //must still check for 2>> or just 2>
      strsep(&err, ">");
      //printf("err after strsep: %s\n", err);
      char *err_append = strchr(err, '>');
      if (err_append){
	return 7;
      }
      return 6;
    }
    else { //check for >>
      strsep(&more, ">");
      char * moremore;
      moremore = strchr(more, '>');
      if (moremore){//has >>
	return 4;
      }
      return 3;//only >
    }
  }
  else if (pipe) {
    return 5;
  } 
  return 0;
}


