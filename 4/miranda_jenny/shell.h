#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

void execute( char * split_cmds );
char* rmspaces( char *str);
void calculator(char * str);
void printUnicorn();
