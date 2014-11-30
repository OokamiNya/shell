/**
 * shell.h
 * 
 * Header file for shell.c.
 *
 **/

// Include guard is here in case I want to add another file
#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/**
 * int execute ( char** input )
 * ============================
 *
 * Takes an array of strings and executes them as a command,
 * with input[0] as the command and other strings as parameters
 *
 * Parameters:
 *     char** input: Array of strings
 *
 * Return:
 *     0 if successful.
 *
 * -Note-: this return was supposed to be used for a recursive
 * redirect function, but it was never implemented.
 *
 **/
int execute ( char** input );

/**
 * char** parse ( char* input, char* delim )
 * =========================================
 *
 * Takes a string and parse it by the delimiter.
 * 
 * Parameters:
 *     char* input: String to be parsed.
 *     char* delim: Delimiter.
 *
 * Return:
 *     A pointer to an array of strings after parse, 
 *     with memory allocated.
 *
 **/
char** parse ( char* input, char* delim );

/**
 * char* find_redirect ( char* input )
 * ===================================
 *
 * Takes a string and finds the first occurence of >, < or |
 * 
 * Parameters:
 *     char* input: String to be analyzed.
 *
 * Return:
 *     A pointer to the first char >, < or | found in input.
 *
 * -Note-: use return[0] to get the actual char.
 *     
 **/
char* find_redirect( char* input );

/**
 * int main()
 * ==========
 *
 * Main method.
 *
 * Process of input/output:
 *     1. Get the console input
 *     2. Parse by ; and execute individual commands
 *     3. Check: Is there a redirect in there?
 *         Yes:
 *             a. Check which redirect
 *             b. Parse by space and execute accordingly.
 *         No:
 *             a. Execute accordingly.
 *     4. Free the memory.
 *     5. Repeat.
 *
 **/
int main();

#endif
