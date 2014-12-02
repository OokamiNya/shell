#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
int count_tokens(char *, char);
char **parse_args(char *);
char **parse_commands(char[]);
