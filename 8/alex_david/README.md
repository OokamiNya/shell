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
Cd to folders with "~/" broken (*FIX*)

## Files and Function Headers:
#shell.c
Main Shell File
_______________
int cd()
Inputs: char *address
Returns: 0 if successfully changes directory, otherwise -1

Used to change directory to address
_______________
int shell()
Inputs: NONE
Returns: NONE

The overarching function that keeps the shell continously running and parses the command line inputs
_______________
int execute()
Inputs: char *command
Returns: 1 if successfully executed

Runs single commands such as 'ls' and 'exit', along with their parameters
_______________
int writeTo()
Inputs: char **params, int index
Returns: NONE

