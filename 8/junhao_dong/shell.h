#ifndef SHELL_H
#define SHELL_H

void printPrompt();
void redirect(char *redirect_file);
void executePipe (char **argv);
void executeMisc(char **argv);
void execute(char **argv);
// Free dynamically allocated memory after use
char ** parseInput(char *input, char *tok);
void shell();

#endif
