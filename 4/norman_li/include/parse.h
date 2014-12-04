#ifndef PARSE_H
#define PARSE_H

#include "../include/list.h"

// Functions
char *remove_trailing_whitespace(char *);       // Remove Whitespace Function
char *remove_extra(char *);                     // Remove Extra Function
static char **split_array(char *, char *);      // Split Array Function
static struct node* split_list(char *, char *); // Split List function
char **arguementify(char *);                    // Arguementify Function
struct node* split_on_semicolon(char *);        // Split on Semicolon Function
char **split_on_stdout(char *);                 // Split on stdout Function
char **split_on_stdin(char *);                  // Split on stdin Function
char **split_on_append(char *);                 // Split on append Function
struct node* split_on_pipe(char *);             // Split on Pipe Function

#endif
