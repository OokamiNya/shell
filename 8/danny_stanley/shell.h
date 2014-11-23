#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define PROMPT_SIZE 256
#define PATH_SIZE 512
#define TOK_SIZE 1024
#define BUFFER_SIZE 2014

const char *shell_name = "StD";

void shell();
void parse_input(char *);
void execute(char **);
void change_directory(char*);
