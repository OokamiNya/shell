#ifndef SHELL_H
#define SHELL_H

void prompt(char* s);
int countargs(char* s);
void splitcmd(char* s, char** args);
void docmd(char** args);
void gtfo();

#endif
