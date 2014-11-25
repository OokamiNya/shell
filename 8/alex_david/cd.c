#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>


int cd (char* s) {
  char path[1000];
  strcpy (path, s);
  char cwd [256];
  getcwd (cwd, sizeof(cwd));
  printf ("%s", cwd);
 
  strcat (cwd, "/");
  strcat (cwd, s);
  printf ("%s", cwd);
  
  //strcat (cwd, "/0");
  int ret = chdir(cwd);
  printf("\n%d\n", ret);
}

int main () {

  cd ("alex_david");
  return 0;
}
