#include "pipes.h"
void piper(char *s){
  char *cmd = (char *)(malloc(10*sizeof(char)));
  char **piparray = (char **)(malloc(10*sizeof(char)));
  int i = 0;
  while (cmd = strsep(&s,"|")){
    piparray[i] = strip(cmd);
    i++;
  }
  int fd,temp_in;
  fd= open("pin",  O_WRONLY|O_CREAT|O_TRUNC,0777); // stdout
  temp_in= open("pin", O_RDONLY); //stdin
  int f = fork();
  if (!f){ 
    dup2(fd, STDOUT_FILENO);	
    parse_string(piparray[0]);
    close(fd);
    exit(0);
  } else{
    wait(&f); 
    int f2 = fork();
    if (!f2){
      dup2(temp_in, STDIN_FILENO);	
      parse_string(piparray[1]);
      close(temp_in);
      remove_file("pin");
      exit(0);
    }
    else{
      wait(&f2);
    }
  }
}

void remove_file(char* f){
  execlp("rm","rm", f, NULL);
}
