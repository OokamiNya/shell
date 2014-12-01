#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>

char** parse_command(char*);
int check_command(char*);
int exec_command(char**);
int check_and_run(char*);
int redirect(char*, char*);
char* remove_newline(char*);

//int redirect_append(char*, char*);
//int pipiing(char*, char*);
