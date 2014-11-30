#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "shell.h"


int main () {
char raw [256];
char clean[256];
char **args = NULL;
char **cmds = NULL;
int argc;
 int cmdc;
 int i;
 while (1) {
   prompt(raw);
   killspaces(raw,clean);
   cmdc = count(clean, ";");
   cmds = malloc(cmdc * sizeof(char*));
   splitinput(clean,cmds);
   i = 0;
   while(i<cmdc) {
     argc = count(cmds[i], " ");
     args = malloc((argc+1) * sizeof(char*)); //+1 for NULL
     if (strchr (cmds[i], '>')) {
       char **chunk = NULL;
       int cc;
       cc = count (cmds[i], ">");
       //printf ("%d", cc);
       chunk = malloc((cc) * sizeof(char*));
       char *c = clean;
       char *t;
       int j = 0; 
       while ((t = strsep(&c, ">"))) {
	 chunk[j] = t;
	 j++;
       }
       ///printf ("There is > here\n, %s, \n%s", chunk [0], chunk [1]);
       splitcmd(chunk[0],args);
       char str[256];
       strcpy (str, chunk[1]);
       //strcat (str, ".txt");
       //str [strlen(str) - 1] = '\0';
       int f = fork();
       if(f == 0) {
	 
	 int fd = open (str,O_WRONLY|O_CREAT, 0666);
	 dup2 (fd, STDOUT_FILENO);
	 close(fd);
	 execvp (args[0], args);
	 printf ("LMFAO no such command XDDDDDD q:^)-k\n");
	 exit(0);
       } else {
	 wait(1);
       }
       free (chunk);
     }
     if (strchr (cmds[i], '<')) {
       char **chunk = NULL;
       int cc;
       cc = count (cmds[i], "<");
       //printf ("%d", cc);
       chunk = malloc((cc) * sizeof(char*));
       char *c = clean;
       char *t;
       int j = 0; 
       while ((t = strsep(&c, "<"))) {
	 chunk[j] = t;
	 j++;
       }
       ///printf ("There is > here\n, %s, \n%s", chunk [0], chunk [1]);
       splitcmd(chunk[0],args);
       char str[256];
       strcpy (str, chunk[1]);
       //strcat (str, ".txt");
       //str [strlen(str) - 1] = '\0';
       int f = fork();
       if(f == 0) {
	 
	 int fd = open (str,O_RDONLY, 0666);
	 dup2 (fd, STDIN_FILENO);
	 close(fd);
	 execvp (args[0], args);
	 printf ("LMFAO no such command XDDDDDD q:^)-k\n");
	 exit(0);
       } else {
	 wait(1);
       }
       free (chunk);
     }
     else {
       splitcmd(cmds[i],args);
       if (!strlen(cmds[i])) {
	 //no command given, do nothing
       } else if(!strcmp(args[0],"exit")) {
	 gtfo();
       } else if(!strcmp(args[0],"cd")) {
	 if (chdir (args[1])){
	   //will not take flags
	   printf ("no such directory GET FKIN NOSCOPED REKT LMAO\n");
	 }
       } else {
	 int f = fork();
	 if(f == 0) {
	   docmd(args);
	 } else {
	   wait(1);
	 }
       }
     }
     i++;
   }
   free(args);
   free(cmds);
 }
 return 0;
}
