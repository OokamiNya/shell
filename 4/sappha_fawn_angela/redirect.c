#include <fcntl.h>
#include <unistd.h>

void redirect_stdout(char * from, char * to){
  //> 
  //STDOUT_FILENO
  int fd, fd1;
  fd = open(to, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  fd1 = dup(STDOUT_FILENO);
  dup2(fd, STDOUT_FILENO);
  dup2(fd1, STDOUT_FILENO);

}
			  
