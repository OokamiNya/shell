#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


/*
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
*/

char ** parseInput(char *input, char *tok);
void changeDir();
void execute(char **argv);
void shell();
