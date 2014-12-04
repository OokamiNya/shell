#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "shell_test_3.h"

int main(){
  /*char s[50] = "cd ~/Desktop/SYSTEMS_FALL_2014";
  cd(s);
  char t[10] = "ls -l";
  executing(t);
  */
  // char s[20] = "ls -l";
  //executing(s);
  /*char t[20] = "ls -l ; echo hello";
  char ** a;
  a = (char **)malloc(sizeof(char**));

  a = parsing_semi(t);
  int i;
  for (i=0; a[i]; i++){
    printf("a[%d] is: %s\n", i, a[i]);

  }
  executing(a[0]);
  */
  /* int k;
    for(k = 0; a[k]; k++){
      executing(a[k]);
    }

    free(a);
  */
  
 while(1){
   //printf("HOLLAFORADOLLA$ ");
   char path[500];
   getcwd(path, sizeof(path));
   strcat(path, "$");
   printf("%s", path);
   char command[256];
   fgets(command, 256, stdin);

   command[strlen(command)-1] = 0;

   //skips newline
   char * s1 = command;
   char * s = strsep(&s1, "\n");

   int f = fork();
   int status;
   if (f == 0){
     char ** a;
     a = (char **)malloc(sizeof(char **));

     a = parsing_semi(s);

     int i;
     for(i = 0; a[i]; i++){
       exec_all(a[i]);
     }

     free(a);
      exit(0);
    }
    else{
      wait(&status);
    }
 }
  
  return 0;
}
/*
//doesn't work?!?!
char * no_spaces(char * s){
  int len = strlen(s);
  char * ret;
  if (!strcmp (s[len-1], " ")){
    strncpy(*s, ret, len-1);
  }
  if (!strcmp (s[0], " ")){
    strncpy(s[1], ret, len-1);
  }
  return ret;
}
*/

char ** general_parse(char * s, char * c){
  char * s1 = s;
  char * s2;
  char ** a;
  int i = 0;

  a = (char **)malloc(sizeof(char **));

  while (s2 = strsep(&s1, c)){
    a[i] = s2;
    i++;
  }
  s[i] = 0;

  return a;
}

char ** parsing_semi(char * s){
  char ** a;
  char c;

  a = (char **) malloc(sizeof(char **));
  a = general_parse(s, ";");

  return a;
}

void executing(char * s){
  char ** a;
  a = (char **)malloc(sizeof(char **));
  a = general_parse(s, " ");

  int f = fork();
  int status;
  if (f == 0){
    execvp(a[0], a);
  }
  else{
    wait(&status);
  }
}

void stdouting(char * s){
  char ** a;
  a = (char **)malloc(sizeof(char**));
  a = general_parse(s, ">");

  int f = fork();
  int status;

  if (f == 0){
    int fd;
    fd = open(a[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    dup2(fd, STDOUT_FILENO);
    exec_all(a[0]);
    close(fd);
    exit(0);
  }
  else{
    wait(&status);
  }
}

void stdining(char * s){
  char ** a;
  a = (char **)malloc(sizeof(char**));
  a = general_parse(s, "<");

  int f = fork();
  int status;

  if (f == 0){
    int fd;
    fd = open(a[0], O_RDONLY, 0644);
    dup2(fd, STDIN_FILENO);
    exec_all(a[0]);
    close(fd);
    exit(0);
  }
  else{
    wait(&status);
  }
}

void piping(char * s){
  char ** a;
  a = (char **)malloc(sizeof(char**));
  a = general_parse(s, "|");

  int f = fork();
  int status;
  if(f == -1){
    printf("Error forking");
    exit(1); 
  }
  //http://tldp.org/LDP/lpg/node11.html
  if (f == 0){
    //close(STDOUT_FILENO);
    dup2(STDOUT_FILENO, STDIN_FILENO);
    execvp(a[0], a);
  }
  else{
    dup2(STDOUT_FILENO, STDIN_FILENO);
    execvp(a[1], a);
    wait(&status);
  }
}

void cd(char * s){
  int errno = 0;
  char * path;
  char * p = s;
  path = (char *)malloc(sizeof(char *));

  strsep(&p, " ");
  path = strsep(&p, "\n");

  if (strstr(path, "~/") == 0){
    char * homedir = getenv("HOME");
    char * psudopath;
    int i = 0;
    //gets rid of ~/ 
    for(i; i < strlen(path)-2; i++){
      psudopath[i] = path[i + 1];
    }
    path = strcat(homedir, psudopath);
  }

  if (errno != 0){
    printf("NO SUCH DIRECTORY. TRY AGAIN.\n");
  }
  if (chdir(path) != 0){
    printf("NO SUCH DIRECTORY. TRY AGAIN.\n");
  }
  else{
    chdir(path);
  }
  /*strcpy(&s[3], path);
  printf("path is: %s\n", path);
  if(chdir(path) == -1){
    printf("NO SUCH DIRECTORY! TRY AGAIN.\n");
  }
  else{
    chdir(path);
    }*/
}

void exec_all(char * s){
  if(strchr(s, '>'))
    stdouting(s);
  else if (strchr(s, '<'))
    stdining(s);
  else if (strchr(s, '|'))
    piping(s);
  else if (strstr(s, "cd"))
    cd(s);
  else if (strstr(s, "exit"))
    exit(0);
  else
    executing(s);
}
