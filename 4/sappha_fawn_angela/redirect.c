#include <fcntl.h>
#include <unistd.h>
#include <string.h>

  
void redirect_in(char * from, char * to, int mode){
  //> redirecting stdout to a file
  //STDOUT_FILENO
	int fd, fd1;
	fd = open(to, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	fd1 = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	execute(from);
	dup2(fd1, STDOUT_FILENO);
}

void redirection(char *s, int mode){
  char *sep;
  char *in = (char*) malloc((sizeof(char)*256));
  strcpy(in, s);
  
  if (mode == 1){//<
    sep = strsep(&in, "<");
    sep = trim(sep);
    in = trim(in);
    if (in == 0) {//if null
      printf("owl: syntax error near unexpected token `newline'\n");
    }
    else {
      redirect_in(sep, in, 1);
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
