#ifndef SHELL_H
#define SHELL_H

static void sigHandler(int signo);
void printPrompt();
char * trimSpace(char *str);
void redirect();
void executePipe ();
void executeMisc();
void execute();
char ** parseInput(char *input, char *delim);
void shell();

#endif
