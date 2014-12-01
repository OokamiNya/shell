#ifndef SHELL_H
#define SHELL_H

void prompt(char* s);
char is_control_char(char c);
char* killspaces(char* src, char* dst);
int count (char*s, char *c);
int countcmds(char* s);
int countargs(char* s);
void splitinput(char* s, char** cmds);
void splitcmd(char* s, char** args);
void docmd(char** args, int redir_in, int redir_out);
void gtfo();

#endif
