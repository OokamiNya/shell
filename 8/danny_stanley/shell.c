#include "shell.h"
#include "colors.h"

int errno_result; // Used in collaboration with errno if function fails
pid_t child_pid;
char *prompt;

int args;
char **argv;
int tokIndex;
int tokSize;
char *tok;
int escapeIndex;
int escapeSize;
char *escape_buf;

int cmd_status = 1;
int valid_input = 0;

node* path_history; //For cd history

static void signalhandler(int signal) {
    if (signal == SIGINT) {
        if (child_pid) {
            kill(child_pid, SIGINT);
            int status;
            waitpid(child_pid, &status, WNOHANG | WUNTRACED); // Wait for child to end
            create_prompt(prompt, PROMPT_SIZE);
            valid_input = 1;
            printf("\n%s", prompt); // Printout a new prompt
            fflush(stdout);
        }
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
    if (escape_buf) { // Frees escape_buf if it is already defined
        free(escape_buf);
        escape_buf = NULL;
    }
}

void setup_argv() {
    argv = (char **) malloc(sizeof(char *));
    args = 0;
    tokSize = TOK_INIT_SIZE;
    tokIndex = 0;
    tok = (char *) malloc(TOK_INIT_SIZE);
}

/* Given the input from user, and index in input
 * Returns number of characters read
 * NOTE: This only handles one escape sequence!
 * REMEMBER TO FREE escape_buf after usage! */
int escape_read(char *input, int index) {
    int initIndex = index;
    escapeIndex = 0;
    escapeSize = ESCAPE_SIZE;
    escape_buf = (char *) malloc(ESCAPE_SIZE);
    char isEscaping = 0;
    int done = 0;
    while (input[index] && !done) {
        if (input[index] == '\\') { // Escape any character after '\'
            ++index;
            escape_buf[escapeIndex] = input[index];
            ++escapeIndex;
            if (!isEscaping) {
                done = 1;
            }
            isEscaping = 0;
        }
        else if (input[index] == '\'') {
            if (isEscaping == '\'') { // If already escaping '\'', then stop it
                isEscaping = 0;
                done = 1;
            }
            else if (isEscaping) { // Escape the '\''
                escape_buf[escapeIndex] = input[index];
                ++escapeIndex;
            }
            else { // Set escaping if no current escape
                isEscaping = '\'';
            }
        }
        else if (input[index] == '\"') {
            if (isEscaping == '\"') { // If already escaping '\"', then stop it
                isEscaping = 0;
                done = 1;
            }
            else if (isEscaping) { // Escape the '\"'
                escape_buf[escapeIndex] = input[index];
                ++escapeIndex;
            }
            else { // Set escaping if no current escape
                isEscaping = '\"';
            }
        }
        else { // If not escape character, do normal escaping
            escape_buf[escapeIndex] = input[index];
            ++escapeIndex;
        }
        ++index;

        if (escapeIndex >= escapeSize-2) { // Subtract 2 to be safe in expanding escape_buf
            escapeSize += ESCAPE_SIZE;
            escape_buf = (char *) realloc(escape_buf, escapeSize * sizeof(char));
        }
    }
    escape_buf[escapeIndex] = '\0';
    return index - initIndex;
}

int main() {
    signal(SIGINT, signalhandler);
    child_pid = fork(); // Hack to initialize child_pid as a value that does not disrupt the system
    if (!child_pid) {
        exit(0);
    }
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
                if (args != 0 || tokIndex != 0) { // Makes sure that there is something to execute
                    if (tokIndex != 0) { // Adds last token to argv
                        tok[tokIndex] = '\0';
                        ++args;
                        argv = (char **) realloc(argv, args * sizeof(char *));
                        argv[args-1] = strdup(tok);
                    }
                }
                else { // If there is a blank redirection, they want cat functionality!
                    ++args;
                    argv = (char **) realloc(argv, args * sizeof(char *));
                    argv[args-1] = strdup("cat");
                }
                argv = (char **) realloc(argv, (args + 1) * sizeof(char *)); // NULL is needed for execvp
                argv[args] = NULL;

                ++index;
                int mode; // Selecting whether to append or write
                if (input[index] == '>') {
                    ++index;
                    mode = O_APPEND;
                }
                else {
                    mode = O_TRUNC;
                }

                while (input[index] == ' ') { // Remove prepending whitespace from filename
                    ++index;
                }
                int fileSize = FILE_SIZE;
                int fileIndex = 0;
                char *filename = (char *) malloc(FILE_SIZE * sizeof(char));
                while (input[index] && input[index] != ' ' && input[index] != ';') {
                    if (input[index] == '\\' || input[index] == '\'' || input[index] == '\"') { // Handle escapes in filename
                        index += escape_read(input, index);
                        if (fileIndex + escapeIndex + 2 >= fileSize) { // Expand filename to fit escaped characters
                            fileSize += escapeIndex + FILE_SIZE;
                            filename = (char *) realloc(filename, fileSize * sizeof(char));
                        }
                        strcpy(filename + fileIndex, escape_buf); // Copy escape_buf to end of filename
                        fileIndex += escapeIndex;
                    }
                    else {
                        filename[fileIndex] = input[index];
                        ++fileIndex;
                        ++index;
                    }
                    if (fileIndex + 2 > fileSize) {
                        fileSize += FILE_SIZE;
                        filename = (char *) realloc(filename, fileSize * sizeof(char));
                    }
                }
                filename[fileIndex] = '\0';
                int output = open(filename, O_CREAT | O_WRONLY | mode, 0644); // Open file for redirection
                if (output == -1) {
                    printf("Redirecting to file failed: %s\n", strerror(errno));
                }
                else {
                    int stdout_backup = dup(STDOUT_FILENO);
                    dup2(output, STDOUT_FILENO); // Redirects stdout to file
                    execute(argv);
                    close(output);
                    dup2(stdout_backup, STDOUT_FILENO); // Restores stdout
                }
                cleanup_argv(); // Clean up redirection commands, so that they don't run again later on
                setup_argv();
                --index; // Offset ++index at the end of this while loop
                valid_input = 2; // Saves into history
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
                valid_input = 2; // Saves into history
            }
            else if (input[index] == '\\' || input[index] == '\'' || input[index] == '\"') {
                index += escape_read(input, index); // Increase index by number of characters read in escape_read()
                if (tokIndex + escapeIndex + 2 >= tokSize) {
                    tokSize += escapeIndex + TOK_INIT_SIZE;
                    tok = (char *) realloc(tok, tokSize * sizeof(char)); // Expand tok to fit escape_buf
                }
                strcpy(tok + tokIndex, escape_buf); // Copies escape_buf to end of tok
                tokIndex += escapeIndex;
                --index; // Move back in index pointer since escape_read() goes to after escape is done, offset needed for ++index at end of while loop
            }
            else if (input[index] == '~') {
                if (input[index+1] == '/') { // Replace ~ with $HOME when referring to directories
                    char *home = getenv("HOME");
                    strcpy(tok + tokIndex, home);
                    tokIndex += strlen(home);
                }
                else { // Replace ~user with home directory of user
                    ++index; // Move past '~'
                    int userSize = USER_SIZE;
                    int userIndex = 0;
                    char *user = (char *) malloc(USER_SIZE * sizeof(char));
                    while (input[index] && input[index] != ' ' && input[index] != ';' && input[index] != '>' && input[index] != '|') {
                        if (input[index] == '\\' || input[index] == '\'' || input[index] == '\"') { // Handle escapes in user
                            index += escape_read(input, index);
                            if (userIndex + escapeIndex + 2 >= userSize) { // Expand user to fit escaped characters
                                userSize += escapeIndex + FILE_SIZE;
                                user = (char *) realloc(user, userSize * sizeof(char));
                            }
                            strcpy(user + userIndex, escape_buf); // Copy escape_buf to end of user
                            userIndex += escapeIndex;
                        }
                        else {
                            user[userIndex] = input[index];
                            ++userIndex;
                            ++index;
                        }
                        if (userIndex + 2 > userSize) {
                            userSize += FILE_SIZE;
                            user = (char *) realloc(user, userSize * sizeof(char));
                        }
                    }
                    user[userIndex] = '\0';
                    struct passwd *found_user = getpwnam(user);
                    char *user_home;
                    if (found_user) {
                        user_home = found_user->pw_dir; // Retrieves user home directory
                    }
                    else { // Defaults to printing out user input
                        user_home = user;
                        tok[tokIndex] = '~';
                        ++tokIndex;
                    }
                    strcpy(tok + tokIndex, user_home);
                    tokIndex += strlen(user_home);
                    --index; // Offset ++index at end of while loop
                }
            }
            else {
                tok[tokIndex] = input[index];
                ++tokIndex;
            }
            if (tokIndex >= tokSize) { // Expand buffer for tok
                tokSize += TOK_INIT_SIZE;
                tok = realloc(tok, tokSize * sizeof(char));
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
        if (valid_input != 2) { // 2 is for bypass
            valid_input = 0;
        }
    }
    printf("Token Size: %d\n", tokSize);
    cleanup_argv();
}

void execute(char **argv) {
    /* DEBUG */
    int i = 0;
    for (; i <= args; ++i) {
        printf("$argv[%d]: `%s`\n", i, argv[i]);
    }
    printf("--------------------------------------------------\n");

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
        child_pid = fork();
        if (child_pid) { // Parent process to wait for child to finish
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
