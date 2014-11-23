#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


void process(char * start);
void execute(char * start);
void child_process(char * args[]);
void normal_process(char * args[]);
void redirect(char * args[], int redir);
