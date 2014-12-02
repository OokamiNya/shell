#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

void redir_in(char *input_file, char* executable);
void redir_out(char *output_file, char *exec_args[], char *command, int type);