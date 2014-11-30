#include "redirect.h"

//later add 2> and pipes  

//redirects stdin from a file
void redirect_in(char * command, char * file, int mode){
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

//redirects stdout to a file
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
	dup2(fd1, STDOUT_FILENO);//reset: redirect fd1 to stdout
}

//redirects stdout to a file
void redirect_err(char * command, char * file, int mode){
  int fd, fd1;
  if (mode == 1){ //2>
    fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  }
  else if (mode == 2){ //2>>
    fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
  }
  fd1 = dup(STDERR_FILENO);//set fd1 to stdout
  dup2(fd, STDERR_FILENO);//redirect fd to stdout
  execute(command);
  dup2(fd1, STDERR_FILENO);//reset: redirect fd1 to stdout
}

void pipeify(char * first, char * second) {
  //ls | wc --> ls is first, wc is second
  first = trim(first);
  second = trim(second);
  int stdin_fd, stdout_fd, fd;

  //for resetting purposes
  stdin_fd = dup(STDIN_FILENO);
  stdout_fd = dup(STDOUT_FILENO);

  fd = open("temp", O_CREAT | O_TRUNC | O_WRONLY, 0644);

  dup2(fd, STDOUT_FILENO); //redirects fd to stdout (stdout goes to fd)
  execute(first);//actually writing to fd
  close(fd);
  dup2(stdout_fd, STDOUT_FILENO);//reset
  
  fd = open("temp", O_RDONLY, 0644);
  dup2(fd, STDIN_FILENO); //redirecting fd to stdin (stdin is fd)
  execute(second);
  dup2(stdin_fd, STDIN_FILENO);//reset
  execute("rm temp");
 
  int status, error, f;
  f = fork();
  if (f){//parent
    //printf("parent me!\n");
    wait(&status);
    //printf("error? %d\n", error);
  }
  else { //child
    //printf("child me!\n");
    error = execlp("rm", "rm", "temp", NULL);
  }
 
}

//redirects stdin from a file
void redirection(char *s, int mode){
  char *sep;
  char* in = (char*)malloc((sizeof(char)*256));
  strcpy(in, s);
  if (mode == 1){//<
    sep = strsep(&in, "<");
    sep = trim(sep);
    in = trim(in);
    if (in == 0) {//if null; for example ls < 
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      //printf("in: %s\n", in);
      //printf("sep: %s\n", sep);
      redirect_in(sep, in, 1);
    }
  }
  else if (mode == 3){ //>
   
    sep = strsep(&in, ">");
    sep = trim(sep);
    in = trim(in);
    if (in == 0) {//if null; for example ls < 
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      redirect_out(sep, in, 1);
    }
    
  }
  
  else if (mode == 4){ //>>
    sep = strsep(&in, ">");
    printf("sep: %s\n", sep);
    strsep(&in, ">");
    in = trim(in);
    printf("in (file):%s\n", in);
    if (in == 0) {//if null; for example ls < 
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      //in is file
      redirect_out(sep, in, 2);
    }
  }
  else if (mode == 5) { // |
    sep = strsep(&in, "|");
    sep = trim(sep);
    in = trim(in);
    if (in == 0) { // if there isn't a 2nd command
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      pipeify(sep, in);
    }
  }
  /*
  //FIXIFY
  else if (mode == 6){ //2>
   
    sep = strsep(&in, ">");
    sep = trim(sep);
    in = trim(in);
    if (in == 0) {//if null; for example ls < 
      printf("owl: syntax error near unexpected token newline'\n");
    }
    else {
      redirect_out(sep, in, 1);
    }
  }
  */
  
}
//returns 0 if no redirect symbols
//returns 1 if <
//return 3 if >
//return 4 if >>
//return 5 if |
//return 6 if 2>
//return 7 if 2>>
int has_redirect(char* i){
  //printf("input: %s\n", input);
  char *input = (char*) malloc((sizeof(char)*256));
  strcpy(input, i);
  char *less, *more, *pipe;
  less = strchr(input, '<');
  //printf("less: %d\n", less);
  more = strchr(input, '>');
  //printf("more: %d\n", more);
  pipe = strchr(input, '|');
  if (less) { //there is a < sign
    return 1;
  }
  else if (more) { //there is a < sign
    char *err_check = (char*) malloc((sizeof(char)*256));
    strcpy(input, i);
    char *err = strstr(err_check, "2>");
    if (err) { //has 2>
      //check for 2>>
      strsep(&err, ">");
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
      if (moremore){
	return 4;
      }
      return 3;
    }
  }
  else if (pipe) {
    return 5;
  } 
  return 0;
}


