#include <fcntl.h>

void redirect_stdout(char * from, char * to){
  //> 
  //STDOUT_FILENO
  int fd;
  fd = open(to, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  
}
			  
