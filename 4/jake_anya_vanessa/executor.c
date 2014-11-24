#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int execute(char s[256]) {

  char *s1 = s;
  char *s2;
  char *arg[256];
  
  int i = 0;
  while(s1) {
    s2 = strsep(&s1, " ");
    arg[i] = s2;
    i++;
  }
  arg[i] = 0;
  char * newstr;
  newstr = strsep(&(arg[i-1]), "\n");
  arg[i-1] = newstr;
  
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
