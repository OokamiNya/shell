#include "shell.h"


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
	fd1 = dup(STDOUT_FILENO);
	//printf("DONE");
	dup2(fd, STDOUT_FILENO);
	execute(command);
	dup2(fd1, STDOUT_FILENO);
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
      printf("owl: syntax error near unexpected token `newline'\n");
    }
    else {
      //printf("in: %s\n", in);
      //printf("sep: %s\n", sep);
      redirect_in(sep, in, 1);
    }
  }

  else if (mode == 2){ //<<
    //how do lol
  }

  else if (mode == 3){ //>
   
    sep = strsep(&in, ">");
    sep = trim(sep);
    in = trim(in);
    if (in == 0) {//if null; for example ls < 
      printf("owl: syntax error near unexpected token `newline'\n");
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
      printf("owl: syntax error near unexpected token `newline'\n");
    }
    else {
      //in is file
      redirect_out(sep, in, 2);
    }
  }


}
//returns 0 if no redirect symbols
//returns 1 if <
//returns 2 if <<
//return 3 if >
//return 4 if >>
int has_redirect(char* i){
  //printf("input: %s\n", input);
  char *input = (char*) malloc((sizeof(char)*256));
  strcpy(input, i);
  char *less, *more;
  less = strchr(input, '<');
  //printf("less: %d\n", less);
  more = strchr(input, '>');
  //printf("more: %d\n", more);
  //if neither are in
  if (less) { //there is a < sign
    strsep(&less, "<");
    char * lessless;
    lessless = strchr(less, '<');
    if (lessless){
      return 2;
    }
    return 1;
  }
  
  else if (more) { //there is a < sign
    strsep(&more, ">");
    char * moremore;
    moremore = strchr(more, '>');
    if (moremore){
      return 4;
      }
    return 3;
  }
    
  return 0;

}
