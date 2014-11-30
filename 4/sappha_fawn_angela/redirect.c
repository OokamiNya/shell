#include <fcntl.h>
#include <unistd.h>

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
//else returns 0
//how to handle double redirection			  
int has_redirect(char * input){

  return 0;
}
