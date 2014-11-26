#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>


char *strip(char *);
void parse_string(char *);
void exec(char **,int);
void shell();
void redirect(char *);
static void sighandler(int);
