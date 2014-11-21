#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){

  printf("$ ");
  //PARSING BY ;
  
  char commands[256];
  fgets(commands, 256, stdin);
  
  commands[strlen(commands) - 1] = 0;

  char * c1 = commands;
  char * c2;
  char * b[256];
  int j = 0;

  while (c2 = strsep(&c1, ";")){
    b[j] = c2;
    j++;
  }

  commands[j] = 0;
  //GOTTA FORK THEN EXEC!!!!!
  int i = 0;
  for (i; i < j; i++){
    printf("b[%d] is: %s\n", i, b[i]);
  }
  execvp(b[0], b);
  // int k = 0;
  //execvp(b[0], b);
  /*
  for (k; k < j; k++){
    execvp(b[0], b);
    }*/
  
  /*  
  //EXEC
  char s[256];
  fgets(s, 256, stdin);
  printf("Your input: %s\n", s);

  s[strlen(s)-1] = 0;

  char * s1 = s;
  char * s2;
  char * a[256];
  int i = 0;

  while (s2 = strsep(&s1, " ")){
    a[i] = s2;
    i++;
  }

  s[i] = 0;
  execvp(a[0], a);

  int k = 0;
  for (k; k < i; k++){
    printf("a[k] is: %s\n", a[k]);
    }
  */

  return 0;

}
