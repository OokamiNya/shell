#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** parse(char* input, char* s); //Parses input
char* getinput(char* input); //Gets input from command line and puts it in a string