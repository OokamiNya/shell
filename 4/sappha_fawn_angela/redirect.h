#include "shell.h"

void redirect_in(char * command, char * file);

void redirect_out(char * command, char * file, int mode);

void redirect_err(char * command, char * file, int mode);

void redirection(char *s, int mode);

int has_redirect(char* i);

void pipeify(char * first, char * second);
