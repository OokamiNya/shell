#ifndef SHELL_H
#define SHELL_H

static void sigHandler(int signo);
void printPrompt();
void safe_exec();
char * trimSpace(char *str);
void redirect();
void executePipe(int pipeIndex);
void executeMisc();
void execute();
char ** parseInput(char *input, char *delim);
void shell();

#endif
