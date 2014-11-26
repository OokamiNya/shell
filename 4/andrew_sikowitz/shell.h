#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>

char isRed(char c);
char * clean(char * input);
int parse(char * input);
char ** sparse(char * command);
int separate(char * input);
int run(char ** commands, int c);
