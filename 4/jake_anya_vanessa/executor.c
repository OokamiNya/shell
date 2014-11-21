#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int execute(char s[256]) {

  char *s1 = s;
  char *s2;
  char *arg[256];
  
  int i = 0;
  while(s2) {
    s2 = strsep(&s1, " ");
    arg[i] = s2;
    i++;
  }
  arg[i+1] = 0;
  
  
  printf("%s\n",arg[0]);
  printf("%d\n",strcmp(arg[0],"cd"));

  if(!strcmp(arg[0],"cd")){
    if (sizeof(arg) / sizeof(char *) > 1)
      chdir(arg[1]);
    else
      chdir(getenv("HOME"));
  }
  else{
    execvp(arg[0], arg);
  }
  
  return 0;
}
