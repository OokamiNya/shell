#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

// Constants
#define INPUT_BUF_SIZE 1024
#define DIR_NAME_MAX_SIZE 768
#define TRUE 1
#define FALSE 0
// positive or zero cmd_error means successful execution
#define CMD_ERROR -1
#define CMD_BLANK -2
#define CMD_OKAY 0
#define CMD_FINISHED 1
#define CMD_ERROR_SIGNAL SIGUSR1
#define EOF_EXIT_CODE 10
#define SIGINT_EXIT_CODE 11
#define CMD_SUBSTITUTION_FAIL_EXIT_CODE 12
#define PIPE_FAIL_EXIT_CODE 13
#define MAX_CMD_SUBSTITUTION_SIZE 1024
#define CMD_SUBSTITUTION_BUF_SIZE 512
#define PIPE_TARGET_BUF_SIZE 512
#define NO_FD -1

// Shell built-in functions
static const char *cmd_exit = "exit";
static const char *cmd_cd = "cd";
static const char *cmd_back = "back";

// Parsing states
static const char STATE_NORMAL = 0;
static const char STATE_IN_SINGLE_QUOTES = 1;
static const char STATE_IN_DOUBLE_QUOTES = 2;
static const char STATE_CMD_SUBSTITUTION = 3;
static const char STATE_REDIR_STDOUT_TO_FILE = 4;
static const char STATE_REDIR_APPEND_STDOUT_TO_FILE = 5;
static const char STATE_REDIR_FILE_TO_STDIN = 6;

// Parsing state terminating delimiters
static const char *TERM_DELIM_STATE_REDIR_STDOUT_TO_FILE = " ;<|>\n";
static const char *TERM_DELIM_STATE_REDIR_APPEND_STDOUT_TO_FILE = " ;<|>\n";
static const char *TERM_DELIM_STATE_REDIR_FILE_TO_STDIN = " ;<|>\n";

// Function type signatures
static void sighandler(int signo);
static void readline_sigint_handler();
void print_error();
int save_stdin();
int save_stdout();
int save_stderr();
int save_default_fds();
int restore_stdin();
int restore_stdout();
int restore_stderr();
int restore_default_fds();
void cd(const char *target);
void cd_back();
void execute();
void reset_global_pipes();
void reset_execute_variables();
void free_all();
void parse_input(char input[INPUT_BUF_SIZE]);
void get_stdout_execute(char *container, size_t container_size);

// Variables
extern char cmd_error;
extern int child_pid, rl_child_pid;
extern const char *home;
extern char input[INPUT_BUF_SIZE];
extern char **opts;
extern char *tok;
extern int optCount, tokIndex;
extern char old_pwd[DIR_NAME_MAX_SIZE];
extern char keep_alive;
extern char debug_output;
extern int cmd_nest_level;
extern char *cmd_substitution_buffer;
extern size_t cmd_substitution_buffer_index;
extern int global_pipes[2];

