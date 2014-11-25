#ifndef SHELL_H
#define SHELL_H

void prompt(char* s);
int countcmds(char* s);
int countargs(char* s);
void splitinput(char* s, char** cmds);
void splitcmd(char* s, char** args);
void docmd(char** args);
void gtfo();

#endif
