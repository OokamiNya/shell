#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "jksh.h"
#include "parser.h"
#include "ui.h"

void sig_handler(int signum) {
        switch (signum) {
                case SIGINT:
                        printf("\n");
                        exit(1);
                        break;
        }
}

void setup_sighandler() {
        struct sigaction sa;

        sa.sa_handler = sig_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;

        if (sigaction(SIGINT, &sa, NULL) == -1)
                printf("Couldn't assign handler for SIGINT\n");
}

int main() {
        setup_sighandler();

        const int bufsize = 1024;
        char *line = malloc(sizeof(char) * bufsize + 1);
        char **lines;
        char **tokens;
        int num_tokens;

        while (1) {
                /* Retrieve raw input */
                get_line(line, bufsize);

                /* Split raw input into lines */
                lines = split_into_lines(line);

                /* Parse lines into tokens */
                num_tokens = count_tokens(line);
                tokens = tokenize(line);
                tokens[num_tokens] = NULL;

                execute(tokens, num_tokens);

                /* TODO: free lines */
                /* TODO: free tokens */
        }

        free(line);
        return 0;
}

/* Attempts to execute commands given through tokenized input.*/
void execute(char **tokens, int num_tokens) {
        /* Handle special cases cd and exit */
        if (handle_special_case(tokens, num_tokens))
                return;

        /* Execute command */
        int child_pid = fork();
        if (child_pid == 0) {
                execvp(tokens[0], tokens);

                /* If execvp returns, the command has failed. */
                printf("jksh: command not found: %s\n", tokens[0]);
                exit(1);
        } else {
                /* Ensure the child finishes, then continue */
                int return_status;
                waitpid(child_pid, &return_status, 0);
                if (return_status)
                        printf("Exited with %i\n", return_status);
        }
}

/* cd and exit must be handled in the parent instead of executed in a child
 * process.
 *
 * Returns 0 if no special case was handled. Returns non-zero or exits
 * otherwise */
int handle_special_case(char **tokens, int num_tokens) {
        if (strcmp(tokens[0], "cd") == 0) {
                if (num_tokens != 2)
                        printf("cd requires exactly one argument\n");
                else
                        chdir(tokens[1]);
                return(1);
        }

        if (strcmp(tokens[0], "exit") == 0) {
                printf("exit");
                exit(0);
        }

        return 0;
}