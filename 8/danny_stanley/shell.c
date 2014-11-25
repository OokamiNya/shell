#include "shell.h"
#include "colors.h"

int errno_result; // Used in collaboration with errno if function fails
char *prompt;

int args;
char **argv;
int tokIndex;
int tokSize;
char *tok;

int cmd_status = 1;
int valid_input = 0;

node* path_history; //For cd history

static void signalhandler(int signal) {
    switch(signal) {
    }
}

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
    if (cmd_status) { // Previous command was successful
        snprintf(prompt_buf, prompt_size, "%s%s%s: %s%s%s %s%s\nᐅ %s", bold_prefix, fg_gray, shell_name, bold_prefix, fg_cyan, path, bold_prefix, fg_green, reset);
    }
    else {
        snprintf(prompt_buf, prompt_size, "%s%s%s: %s%s%s %s%s\nᐅ %s", bold_prefix, fg_gray, shell_name, bold_prefix, fg_cyan, path, bold_prefix, fg_red, reset);
    }
    return prompt_buf;
}

void cleanup_argv() {
    --args; // Initially subtract 1 from args to make it zero-based
    for (; args >= 0; --args) {
        free(argv[args]);
    }
    free(argv);
    free(tok);
}

void setup_argv() {
    argv = (char **) malloc(sizeof(char *));
    args = 0;
    tokSize = TOK_INIT_SIZE;
    tokIndex = 0;
    tok = (char *) malloc(TOK_INIT_SIZE);
}

int main() {
    signal(SIGINT, signalhandler);
    prompt = (char *) malloc(PROMPT_SIZE);
    char current_path[PATH_SIZE];
    path_history = insert_node(path_history, getcwd(current_path,PATH_SIZE));//Will make more elegant later
    printf("LL:%s\n", get_arg(path_history));
    while (!feof(stdin)) {
        create_prompt(prompt, PROMPT_SIZE);
        cmd_status = valid_input = 1;
        char *line = readline(prompt);
        if (line == NULL) {
            printf("\n`EOF Sent`\n");
            free(line);
            free(prompt);
            exit(0);
        }
        printf("$input: `%s`\n", line);
        parse_input(line);
        if (valid_input) { // Inputs that contain non-whitespace characters
            add_history(line);
        }
        free(line);
    }
    free(prompt);
    printf("Thanks for using %s!\n", shell_name);
    return 0;
}

void parse_input(char *input) {
    int index = 0;
    setup_argv();
    tok[0] = '\0';
    while (input[index]) {
        if (input[index] != ' ' && input[index] != '\n') {
            if (0) { // Handler for other cases
            }
            else if (input[index] == '>') {
                if (input[index+1] == '>') {
                    // TODO: Implement append
                }
                else {
                    
                }
            }
            else if (input[index] == ';') {
                if (args != 0 || tokIndex != 0) { // Makes sure that there is something to execute
                    if (tokIndex != 0) { // Adds last token to argv
                        tok[tokIndex] = '\0';
                        ++args;
                        argv = (char **) realloc(argv, args * sizeof(char *));
                        argv[args-1] = strdup(tok);
                    }
                    argv = (char **) realloc(argv, (args + 1) * sizeof(char *)); // NULL is needed for execvp
                    argv[args] = NULL;
                    execute(argv);
                    cleanup_argv();
                    setup_argv();
                }
            }
            else if (input[index] == '\\') {
                ++index; // Move on to add next character right after '\'
                tok[tokIndex] = input[index];
                ++tokIndex;
            }
            else if (input[index] == '"') {
                ++index; // Go past "
                while (input[index] && input[index] != '"') { // Continues interpreting as string literal until next '"'
                    tok[tokIndex] = input[index];
                    ++tokIndex;
                    ++index;
                }
            }
            else if (input[index] == '~') {
                if (input[index+1] == '/') { // Replace ~ with $HOME when referring to directories
                    char *home = getenv("HOME");
                    strcpy(tok + tokIndex, home);
                    tokIndex += strlen(home);
                }
                else { // Replace ~user with home directory of user
                    char user[USER_SIZE];
                    int userIndex = 0;
                    while (input[index+1] && input[index+1] != ' ' && input[index+1] != '/' && input[index+1] != ';' && input[index+1] != '>' && input[index+1] != '|') { // +1 to index since we are "looking ahead"
                        user[userIndex] = input[index+1];
                        ++userIndex;
                        ++index;
                    }
                    user[userIndex] = '\0';
                    struct passwd *found_user = getpwnam(user);
                    char *user_home;
                    if (found_user) {
                        user_home = found_user->pw_dir;
                    }
                    else {
                        user_home = user;
                    }
                    strcpy(tok + tokIndex, user_home);
                    tokIndex += strlen(user_home);
                }
            }
            else {
                tok[tokIndex] = input[index];
                ++tokIndex;
            }
            if (tokIndex >= tokSize) { // Expand buffer for tok
                tokSize += TOK_INIT_SIZE;
                tok = realloc(tok, tokSize);
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
    else {
        valid_input = 0;
    }
    printf("%d\n", tokSize);
    cleanup_argv();
}

void execute(char **argv) {
    /* DEBUG */
    int i = 0;
    for (; i <= args; ++i) {
        printf("$argv[%d]: `%s`\n", i, argv[i]);
    }

    char *cmd = argv[0];
    if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) {
        printf("I'm sad to see you go... :(\n");
        cleanup_argv();
        free(prompt);
        exit(0);
    }
    else if (!strcmp(cmd, "cd")) {
      path_history = change_directory(argv[1] , path_history); // Only parse the first argument in cd
    }
    else {
        pid_t pid = fork();
        if (pid) { // Parent process to wait for child to finish
            int status;
            wait(&status);
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status)) {
                    cmd_status = 0; // Notes that there was an error
                }
            }
        }
        else { // Child process to execute commands
            errno_result = execvp(cmd, argv);
            if (errno_result == -1) {
                printf("%s: command not found: %s\n", shell_name, cmd);
            }
            exit(127); // Exit with an error, only reached if execvp fails
        }
    }
}

node* change_directory(char *path , node* history) {
    if (!path) { // When no path specified, use home
        path = getenv("HOME");
    }
    else if (path[0] == '-') { // TODO: Backtracking directories
      printf("Previous Path: %s\n",get_prev(history)->arg);
      path = get_arg(get_prev(history)); //How do you free this memory
    }
    errno_result = chdir(path);
    if (errno_result == -1) {
        printf("cd: %s: %s\n", path, strerror(errno));
        cmd_status = 0;
    }
    else{
      history = insert_node(history, path);
      printf("Previous Path: %s\n",history->arg);
    }
    return history;
}
