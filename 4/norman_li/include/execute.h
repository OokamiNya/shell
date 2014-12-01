#ifndef EXECUTE_H
#define EXECUTE_H

#include "../include/list.h"

// Functions
void run_commands(struct node*);           // Run Commands Function
static void execute_command(char **);      // Execute Command Function
static void redirect_stdout(char **);      // Redirect stdout Function
static void redirect_stdin(char **);       // Redirect stdin Function
static void redirect_append(char **);      // Redirect Append Function
static void redirect_pipe(struct node*);   // Pipe Function

#endif
