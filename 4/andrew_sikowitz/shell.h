#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>
#include <signal.h>

void sighandler(int signo);
char isRed(char c);
char * clean(char * input);
int parse(char * input);
int separate(char * input);
char ** sparse(char * command);
void ssfree(char ** starstar, int c);
int run(char ** commands, int c);
void main_run(char* hostname, char* cwd, struct passwd *pw);

extern int cpid;
extern char valid;
