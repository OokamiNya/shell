#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>


char* trim(char *);

void printprompt();

void execute();

void cd(char *);

char** parser();

char* get_id();
