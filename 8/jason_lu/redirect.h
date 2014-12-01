#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void redirect(char* file,char** args,char* r);
int printargs(char** args);
char* getname(char** args, char* s);
char cmdarr(char** args, char* s);
char* check(char** args);