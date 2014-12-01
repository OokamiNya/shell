#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

void piping(char * args[], int max);
void start(char * command);
void mid(char * command);
void end(char * command);
