#include "shell.h"
#include "shellutil.h"
#include "prompt.h"

int errno_result; // Used in collaboration with errno if function fails
pid_t child_pid = -111111; // Hack to initialize child_pid as a value that does not disrupt the system
char *prompt;

int args;
char **argv;
int tokIndex;
int tokSize;
char *tok;

node* path_history; //For cd history

static void signalhandler(int signal) {
    if (signal == SIGINT) {
        if (child_pid) {
            kill(child_pid, SIGINT);
            int status;
            waitpid(child_pid, &status, WNOHANG | WUNTRACED); // Wait for child to end
            cmd_status = 0;
            create_prompt(prompt, PROMPT_SIZE);
            printf("\n%s", prompt); // Printout a new prompt
            fflush(stdout);
        }
    }
}

void cleanup_argv() {
    --args; // Initially subtract 1 from args to make it zero-based
    for (; args >= 0; --args) {
        free(argv[args]);
    }
    free(argv);
    free(tok);
    if (escape_buf) { // Frees escape_buf if it is defined
        free(escape_buf);
        escape_buf = NULL;
    }
    if (tilde_buf) { // Frees tilde_buf if it is defined
        free(tilde_buf);
        tilde_buf = NULL;
    }
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
    printf("Starting Directory: %s\n", get_arg(path_history));
    while (!feof(stdin)) {
        create_prompt(prompt, PROMPT_SIZE);
        cmd_status = valid_input = 1;
        char *line = readline(prompt);
        if (line == NULL) {
# ifdef DEBUG
            printf("\n~~~ EOF Sent :\\ ~~~\n");
# endif
            free(line);
            free(prompt);
            exit(0);
        }
#ifdef DEBUG
        printf("$input: `%s`\n", line);
# endif
        parse_input(line);
        if (valid_input) { // Inputs that contain non-whitespace characters
            add_history(line);
        }
        free(line);
    }
    free(prompt);
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
            else if (input[index] == '|') {
                if (args != 0 || tokIndex != 0) { // Makes sure that there is something to execute
                    if (tokIndex != 0) { // Adds last token to argv
                        tok[tokIndex] = '\0';
                        ++args;
                        argv = (char **) realloc(argv, args * sizeof(char *));
                        argv[args-1] = strdup(tok);
                    }
                }
                else { // Stops if there is nothing to pipe
                    printf("There is nothing to pipe from!\n");
                    break;
                }
                argv = (char **) realloc(argv, (args + 1) * sizeof(char *)); // NULL is needed for execvp
                argv[args] = NULL;

                int pipes[2];
                if (pipe(pipes) == -1) { // If pipe creation fails, print error
                    printf("Error with piping: %s\n", strerror(errno));
                }
                else {
                    int stdout_backup = dup(STDOUT_FILENO);
                    dup2(pipes[1], STDOUT_FILENO); // Redirects stdout to write end of pipe
                    execute(argv);
                    cleanup_argv(); // Clean up after execution
                    setup_argv();
                    close(pipes[1]);
                    dup2(stdout_backup, STDOUT_FILENO); // Restores stdout

                    if (global_stdin_backup == STDIN_FILENO) { // Only if stdin is not modified
                        global_stdin_backup = dup(STDIN_FILENO); // Saves original stdin
                    }
                    dup2(pipes[0], STDIN_FILENO); // Redirects read end of pipe to stdin
                }
            }
            else if (input[index] == '<') {
                ++index; // Advance past '<'
                while (input[index] == ' ') { // Skip whitespace between '<' and filename
                    ++index;
                }
                int fileSize = FILE_SIZE;
                int fileIndex = 0;
                char *filename = (char *) malloc(FILE_SIZE * sizeof(char));
                while (input[index] && input[index] != ' ' && input[index] != ';') {
                    if (input[index] == '\\' || input[index] == '\'' || input[index] == '\"') { // Handle escapes in filename
                        index += escape_read(input, index);
                        if (fileIndex + escapeIndex + 2 >= fileSize) { // Expand filename to fit escaped characters
                            fileSize += escapeIndex;
                            filename = (char *) realloc(filename, fileSize * sizeof(char));
                        }
                        strcpy(filename + fileIndex, escape_buf); // Copy escape_buf to end of filename
                        fileIndex += escapeIndex;
                    }
                    else if (input[index] == '~') {
                        index += handle_tilde(input, index);
                        if (fileIndex + tildeIndex + 2 >= fileSize) { // Expand filename to fit tilde expansion
                            fileSize += tildeIndex;
                            filename = (char *) realloc(filename, fileSize * sizeof(char));
                        }
                        strcpy(filename + fileIndex, tilde_buf); // Copy tilde_buf to where '~' would be
                        fileIndex += tildeIndex;
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
                int inputFile = open(filename, O_RDONLY); // Open file for redirection
                if (inputFile == -1) {
                    printf("Input redirection from file `%s` failed: %s\n", filename, strerror(errno));
                }
                else {
                    if (global_stdin_backup == STDIN_FILENO) { // Only if stdin is not modified
                        global_stdin_backup = dup(STDIN_FILENO);
                    }
                    dup2(inputFile, STDIN_FILENO); // Redirects inputFile to stdin
                    close(inputFile);
                }
                --index; // Offset ++index at the end of the while loop
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
                            fileSize += escapeIndex;
                            filename = (char *) realloc(filename, fileSize * sizeof(char));
                        }
                        strcpy(filename + fileIndex, escape_buf); // Copy escape_buf to end of filename
                        fileIndex += escapeIndex;
                    }
                    else if (input[index] == '~') {
                        index += handle_tilde(input, index);
                        if (fileIndex + tildeIndex + 2 >= fileSize) { // Expand filename to fit tilde expansion
                            fileSize += tildeIndex;
                            filename = (char *) realloc(filename, fileSize * sizeof(char));
                        }
                        strcpy(filename + fileIndex, tilde_buf); // Copy tilde_buf to where '~' would be
                        fileIndex += tildeIndex;
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
                    printf("Redirecting to file `%s` failed: %s\n", filename, strerror(errno));
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
                    dup2(global_stdin_backup, STDIN_FILENO); // Always restore stdin
                    dup2(global_stdout_backup, STDOUT_FILENO); // Always restore stdout
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
                index += handle_tilde(input, index);
                if (tokIndex + tildeIndex + 2 >= tildeSize) { // Expand filename to fit tilde expansion
                    tokSize += tildeIndex;
                    tok = (char *) realloc(tok, tokSize * sizeof(char));
                }
                strcpy(tok + tokIndex, tilde_buf); // Copy tilde_buf to where '~' would be
                tokIndex += tildeIndex;
                --index; // Offset ++index at end of while loop
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
    dup2(global_stdin_backup, STDIN_FILENO); // Restores stdin regardless
    dup2(global_stdout_backup, STDOUT_FILENO); // Restores stdout regardless
    piping_state = 0; // Terminates any pipes
    cleanup_argv();
}

void execute(char **argv) {
# ifdef DEBUG
    int i = 0;
    for (; i <= args; ++i) {
        printf("$argv[%d]: `%s`\n", i, argv[i]);
    }
    printf("--------------------------------------------------\n");
# endif

    char *cmd = argv[0];
    if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) {
        cleanup_argv();
        free(prompt);
        exit(0);
    }
    else if (!strcmp(cmd, "cd")) {
# ifdef DEBUG
      printf("path_history PATH IN: %s\n",get_arg(path_history));
# endif
      path_history = change_directory(argv[1] , path_history); // Only parse the first argument in cd
# ifdef DEBUG
      printf("path_history PATH OUT: %s\n", get_arg(path_history));
# endif
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

node * change_directory(char *path , node* history) {
# ifdef DEBUG
  printf("PATH: %s\n",path);
# endif
  if (!path) { // When no path specified, use home
    path = getenv("HOME");
  }
  
  else if (path[0] == '-') { // TODO: Backtracking directories
    //printf("Previous Path: %s\n",history->arg);
    path = get_arg(history);
  }
# ifdef DEBUG
  printf("Path Var: %s\n",path);
# endif
  errno_result = chdir(path);
  if (errno_result == -1) {
    printf("cd: %s: %s\n", path, strerror(errno));
    cmd_status = 0;
  }
  else{
    history = insert_node(history, path);
  }
# ifdef DEBUG
    printf("History Path returned: %s\n",history->arg);
# endif
  return history;
}
