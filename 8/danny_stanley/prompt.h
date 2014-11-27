#pragma once
#include "shell.h"

#define PROMPT_SIZE 2014
#define PATH_SIZE 1024
#define TIME_SIZE 14

char * get_path(char *, int);
char * get_time(char *, int);
char * create_prompt(char *, int);

extern int cmd_status;
extern int valid_input;
