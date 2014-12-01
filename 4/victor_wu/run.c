#include "shell.h"

//>Initiates the terminal and runs the commands necessary to get it started.
//>Takes input and prints out the prompt.	
//>Is a wrapper function, nothing to see here.

int main() {
  char curr_dir[256];
  char* input = (char*)malloc(256*sizeof(char));
  printf("Initiating...\n");
  sleep(1);
  exec_command(parse_command("clear"));
  printf("Welcome to Bash!\n(Please read the manual)\n\n");
  
  while(strcmp(input, "exit")) {
    getcwd(curr_dir, sizeof(curr_dir));
    printf("~%s: ", curr_dir);
    fgets(input, 256, stdin);
    check_and_run(input);
  }

  //printf("Bye...\n");
  //sleep(1);
  //exec_command(parse_command("clear"));
  
  return 0;
}
