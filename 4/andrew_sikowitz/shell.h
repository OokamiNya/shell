#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

char isRed(char c);
char * clean(char * input);
int parse(char * input);
int separate(char * input);
int run(char ** commands);
