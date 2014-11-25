#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main() {
  int x = 1;

  while (x) {
    printf("she_sells_sea_shells$ ");

    //get input
    char s[100];
    fgets (s, sizeof(s), stdin);

    //parse input
    char *s1 = s;
    s1 = strsep(&s1, "\n");
    char *s2;
    char * args[10];
    int c = 0;
    while ((s2 = strsep(&s1, " "))) {
      args[c] = s2;
      c++;
    }
    args[c] = NULL;

    //exit
    if (!strcmp(args[0], "exit")) {
      x = 0;
      //char s[26];
      //sprintf(s,"%d", getpid());
      //execlp("kill", "kill", "-9", s, NULL);
      
    }

    //cd
    else if ( !strcmp(args[0], "cd") ) {
      // d = opendir(args[1]);
    }

    else {
      //fork
      int f = fork();
      
      if (f) {
	wait();
      }
      
      if (!f) {
	execvp(args[0], args);
      }
    }


  }

  return 0;
}
