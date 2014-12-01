#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

//These are going to be used with piping.
#define READ_END 0
#define WRITE_END 1

//Function Pointers
typedef void (* REDIR_FUNC) (char *, int);

int process_redir( char * );
void append_file(char *, int);
void trunc_file(char *, int);
void append_all(char *, int);
void trunc_all(char *, int);
