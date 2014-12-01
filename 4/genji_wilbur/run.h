#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "redir.h"

int run( char *, char *, int[], int);
int run_exec( char *, char *, int[], int);
int run_exit( char *, char *, int[], int );
int run_cd( char *, char *, int[], int );
