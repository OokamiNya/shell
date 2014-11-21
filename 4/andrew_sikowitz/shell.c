#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#define pwd pw->pw_dir

char isRed(char c) {
  return c == '>' || c == '<' || c == '|';
}

char * clean(char * input) {
  char * ph = input;
  while (*ph && *ph == ' ') {
    input++;
    ph++;
  }
  
  int i;
  int length = strlen(ph);
  char x = 0;
  //Shifts to left if more than 1 space
  for (i=0; i<length; i++) {
    if (ph[i] == ' ') {
      if (x) {
	strncpy(ph+i-1, ph+i, strlen(ph)+1);
	i--;
      }
      else
	x = 1;
    }
    else {
      x = 0;
    }
  }
  if (input[strlen(input)-1] == ' ')
    input[strlen(input)-1] = 0;

  return input;
}

int parse(char * input) {
  int c = 1;
  int i, j;
  for (i=0; i<strlen(input); i++) {
    if (isRed(input[i]))
      c++;
  }
  
  char ** commands = (char **) malloc(1024*c);
  char * ph = input;
  
  for (i=0; i<c; i++) {
    commands[i] = (char *) malloc(1024);
    for (j=0; j<strlen(ph) && !isRed(ph[j]); j++) {}
    strncpy(commands[i], ph, j);
    commands[i][j] = 0;
    ph = ph+j;
  }
  printf("HI\n");
  for (i=0; i<c; i++) {
    printf("%d: %s!\n", i, clean(commands[i]));
  }
  return 0;
}    

int main() {
  //Main program
  printf("PID: %d\n", getpid());
  printf("Login: %s\n", getlogin());
  char hostname[4096];
  char cwd[4096];
  char * cwdp;
  getcwd(cwd, 4096);
  gethostname(hostname, 4096);
  struct passwd *pw = getpwuid(getuid());
  printf("Home Directory: %s\n", pwd);
  printf("CWD: %s\n", cwd);
  printf("Hostname: %s\n", hostname);
  while (1) {
    getcwd(cwd, 4096);
    gethostname(hostname, 4096);
    getpwuid(getuid());
    cwdp = strstr(cwd, pwd) + strlen(pwd) - 1;
    *cwdp = '~';
    printf("%s@%s:%s$ ", getlogin(), hostname, cwdp);
    char * input = (char *) malloc(1024);
    fgets(input,1024,stdin);
    input[strlen(input)-1] = 0; //Removes newline
    parse(input);
    free(input);
  }
}
