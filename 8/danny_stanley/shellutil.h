#pragma once
#include "shell.h"

#define USER_SIZE 128
#define ESCAPE_SIZE 128
#define TILDE_SIZE 256

int escape_read(char *, int);
int handle_tilde(char *, int);

process * remove_process(process **, int, int);

extern int escapeIndex;
extern int escapeSize;
extern char *escape_buf;
extern int tildeIndex;
extern int tildeSize;
extern char *tilde_buf;
