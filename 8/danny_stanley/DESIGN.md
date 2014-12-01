Documentation
-------------

## shell.c - Parsing input, execute functions, cleanup and setup

#### static void signalhandler(int signal)
> Handles the `SIGINT` and `SIGQUIT` signals.

#### node * parse_input(char *input, node *path)
> Takes an string and parses it character by character. Accepted special delimitors include `\0`, `~`, `\`, `'`, `"`, `;`, `>`, `>>`, `<`, `|`, `&`.

#### node * execute(char **argv, int isBackground, node *path)
> Takes a list of arguments and runs argv[0]. Also gives the option of running commands in the background.

#### node * change_directory(char *path , node *history)
> Takes a path and changes the cwd to the path. Handles `~` as well. After changing the path, adds the old path to the history so that you can backtrack.

#### void setup_shell()
> Allocates memory for shell and set default values.

#### void cleanup_shell()
> Frees memory so that there are no memory leaks.

#### void setup_argv()
> Sets up the argument array by allocating memory and setting default values.

#### void cleanup_argv()
> Frees memory needed for argument array so no leaky.

#### void add_null_argv()
> Adds the NULL argument needed for the `execvp` call.

## shellutil.c - Utilities for the shell, called multiple times in shell

#### int escape_read(char *input, int index)
> Given an input starting at an escape deliminator, start reading until the end of the escape deliminator and returns the number of charaters read. Sets the value of `escape_buf` to the escaped characters read

#### int handle_tilde(char *input, int index)
> Given an input starting at a tilde, sets the value of tilde_buf to the path of the home directory. If there is a user specified after the ~, sets tilde_buf to the home directory of the user

#### process * remove_process(process **array, int arraySize, int pid)
> Given an array of processes and size, finds the process with the given pid and sets the pointer to the process struct to NULL. Returns the pointer to the process found, -1 if no process found

## prompt.c - Functions to create a nice looking prompt

#### char * get_path(char *path_buf, int path_size)
> Given a buffer, writes to it the home directory of the user using the shell

#### char * get_time(char *time_buf, int time_size)
> Given a buffer, write to it the current time in AM/PM

#### char * create_prompt(char *prompt_buf, int prompt_size)
> Generates the colorful looking prompt

## directorystack.c - Functions for cd backtracking

#### char * get_arg(node * current_node)
> Gets the path value of the node specified

#### node * get_prev(node * current_node)
> Frees the current node and returns a pointer to the previous node if it exists. Returns NULL if there is no previous node

#### node * insert_node(node * current_node, char * input)
> Inserts a node in front of the current_node with its path value being the input given

## colors.h - Variables for colors used in the prompt

Run color_generate.c if you want to see more colors!
