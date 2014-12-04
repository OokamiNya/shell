#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>

int run_command(char* s);
int change_directory(char** args);
int redirect(int i, int r, char** args);
