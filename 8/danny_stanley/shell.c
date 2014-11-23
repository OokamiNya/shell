#include "shell.h"
#include "colors.h"

int errno_result; // Used in collaboration with errno if function fails
char *prompt;
int args;
char **argv;

/* Writes directory path given a buffer */
char * get_path(char *path_buf, int path_size) {
    char *home = getenv("HOME");
    char cwd_buf[path_size];
    getcwd(cwd_buf, path_size);
    char *cwd = cwd_buf;
    char *home_in_cwd = strstr(cwd, home);
    if (home_in_cwd) {
        cwd = cwd + strlen(home) - 1; // Move pointer of path_buf to point to the character right before $HOME ends
        cwd[0] = '~'; // Replace character with ~
    }
    snprintf(path_buf, path_size, "%s", cwd);
    return path_buf;
}

/* Writes prompt given a buffer */
char * create_prompt(char *prompt_buf, int prompt_size) {
    char path[PATH_SIZE];
    get_path(path, PATH_SIZE);
    snprintf(prompt_buf, prompt_size, "%s%s%s: %s%s%s %s%sᐅ %s", bold_prefix, fg_gray, shell_name, bold_prefix, fg_cyan, path, bold_prefix, fg_green, reset);
    return prompt_buf;
}

void cleanup() {
    --args; // Initially subtract 1 from args to make it zero-based
    for (; args >= 0; --args) {
        free(argv[args]);
    }
    free(argv);
}

int main() {
    prompt = (char *) malloc(PROMPT_SIZE);
    while (!feof(stdin)) {
        create_prompt(prompt, PROMPT_SIZE);
        char *line = readline(prompt);
        if (line == NULL) {
            printf("\n`EOF Sent`\n");
            free(line);
            free(prompt);
            exit(0);
        }
        printf("$: `%s`\n", line);
        add_history(line);
        parse_input(line);
        free(line);
    }
    free(prompt);
    printf("Thanks for using %s!\n", shell_name);
    return 0;
}

void parse_input(char *input) {
    argv = (char **) malloc(sizeof(char *));
    args = 0;
    int index = 0;
    char tok[TOK_SIZE];
    int tokIndex = 0;
    tok[0] = '\0';
    while (input[index]) {
        if (input[index] != ' ' && input[index] != '\n') {
            if (0) { // Handler for other cases
            }
            else {
                tok[tokIndex] = input[index];
                ++tokIndex;
            }
        }
        else if (input[index] == ' ' && tokIndex != 0) { // When a tok is terminated by a ' ', checks to make sure there is actually something to terminate first
            tok[tokIndex] = '\0';
            ++args;
            argv = (char **) realloc(argv, args * sizeof(char *)); // Expands argv to fit another arg
            argv[args-1] = strdup(tok);
            tok[0] = '\0'; // Resets tok
            tokIndex = 0;
        }
        ++index;
    }
    if (index != 0 && (args != 0 || tokIndex != 0)) { // To account for tokens right before \n <-- which is \0 because of readline(), also makes sure there is something to execute
        if (tok[0] != '\0') { // Prevents empty space after input
            tok[tokIndex] = '\0';
            ++args;
            argv = (char **) realloc(argv, args * sizeof(char *));
            argv[args-1] = strdup(tok);
        }

        argv = (char **) realloc(argv, (args + 1) * sizeof(char *)); // NULL is needed for execvp
        argv[args] = NULL;
        execute(argv);
    }
    cleanup();
}

void execute(char **argv) {
    /* DEBUG */
    int i = 0;
    for (; i <= args; ++i) {
        printf("$argv[%d]: `%s`\n", i, argv[i]);
    }

    char *cmd = argv[0];
    if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) {
        printf("I'm sad to see you go... :(");
        cleanup();
        free(prompt);
        exit(0);
    }
    else if (!strcmp(cmd, "cd")) {
        change_directory(argv[1]); // Only parse the first argument in cd
    }
    else {
        pid_t pid = fork();
        if (pid) { // Parent process to wait for child to finish
            int status;
            wait(&status);
        }
        else { // Child process to execute commands
            errno_result = execvp(cmd, argv);
            if (errno_result == -1) {
                printf("%s: command not found: %s\n", shell_name, cmd);
            }
            exit(0);
        }
    }
}

void change_directory(char *path) {
    char *path_cpy = path;
    char *home_cpy = strdup(getenv("HOME"));
    if (path_cpy[0] == '~') {
        path_cpy++; // Goes beyond ~
        path_cpy = strcat(home_cpy, path_cpy);
    }
    else if (path_cpy[0] == '\0') { // When no path specified, use home
        path_cpy = home_cpy;
    }
    errno_result = chdir(path_cpy);
    if (errno_result == -1) {
        printf("%s: %s\n", "cd", strerror(errno));
    }
    free(home_cpy);
}
