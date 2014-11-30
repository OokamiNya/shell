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
	dup2(fd1, STDOUT_FILENO);
}


//returns 1 if has redirect symbol (< , >)
//returns 2 if has two redirect symbols right next to each other
//else returns 0 (false)
int has_redirect(char* input){
  //printf("input: %s\n", input);
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
      return 2;
      }
    return 1;
  }
    
  return 0;

}
