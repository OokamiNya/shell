BALD
============
by Alex Libman and David Bang (Period 8 Systems)

## Features:
Forks and executes commands<br>
Parses multiple commands with ;<br>
Redirects with >, >>, and <<br>
Piping with | (*TO DO*)<br>

## Attempted:

## Bugs:
Cd to folders with "~/" broken (*FIX*)<br>

## Files and Function Headers:
#shell.c
Main Shell File
_______________
int cd()<br>
Inputs: char *address<br>
Returns: 0 if successfully changes directory, otherwise -1<br>

Used to change directory to address<br>
_______________
int shell()<br>
Inputs: NONE<br>
Returns: NONE<br>

The overarching function that keeps the shell continously running and parses the command line inputs<br>
_______________
int execute()<br>
Inputs: char *command<br>
Returns: 1 if successfully executed<br>

Runs single commands such as 'ls' and 'exit', along with their parameters<br>
_______________
int writeTo()<br>
Inputs: char **params, int index<br>
Returns: NONE<br>

Writes to a file when '>' is used in a command<br>
_______________
int appendTo()<br>
Inputs: char **params, int index<br>
Returns: NONE<br>

Appends to a file when '>>' is used in a command<br>
_______________
int inputFrom()<br>
Inputs: char **params, int index<br>
Returns: NONE<br>

Grabs input from a file when '<' is used in a command<br>
_______________
int pipeCommands()<br>
Inputs: char *left, char *right<br>
Returns: None<br>

Creates a pipe. Closes read end and parent send output to pipe. Closes write end and child reads from pipe.
_______________
int executePipe()<br>
Inputs: char *s<br>
Returns: None<br>

Execute Commands that will be used in piping.
_______________
int emptyString()<br>
Inputs: char *s<br>
Returns: 1 if s contains no commands, 0 otherwise<br>

Used to parse for empty commands<br>
