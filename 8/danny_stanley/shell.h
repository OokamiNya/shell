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

#define PROMPT_SIZE 1024
#define PATH_SIZE 768
#define TOK_INIT_SIZE 256

const char *shell_name = "StD";

void shell();
void parse_input(char *);
void execute(char **);
void change_directory(char*);
