#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "redirect.h"

int count_tokens(char *line, char delim);
void parse(char *line, int num, char *array[], char *delim);