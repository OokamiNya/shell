#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>
#include<fcntl.h>
#include<signal.h>
#include<errno.h>

static void sighandler(int);
void normal_stuff(char arg[]);
char redirection(char arg[]);
char pipe_it(char arg[]);
char dumb_exceptions(char arg[]);
