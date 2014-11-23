#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <ncurses.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void shell();
int parse_input(char *);
void call_cmd(char *, char *);
void change_directory(char*);
