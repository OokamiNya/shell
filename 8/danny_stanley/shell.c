#include "shell.h"

int errno_result; // Used in collaboration with errno if function fails

void shell() {
    char *path_buf = (char *) malloc(BUFFER_SIZE);
    char *input = (char *) malloc(BUFFER_SIZE);
    int done = 0;
    //Getting the Home Directory
    /* Old SMART way
    uid_t user = getuid();
    struct passwd * pwd;
    pwd = getpwuid(user);
    char* home = pwd->pw_dir;
    */
    //Fast efficient way
    char *home = getenv("HOME"); // Required because somehow the environment variable changes when you chdir ~/Desktop
    while (!done) {
      getcwd(path_buf, BUFFER_SIZE);
      char *home_index = strstr(path_buf, home);
      //printf("%s -- %s\n", path_buf, home);
      if (home_index == NULL) {
	printf("\e[37;1mStD: \e[36;1m%s\e[32;1m ᐅ \e[033;0m", path_buf);
      }
      else {
	printf("\e[37;1mStD: \e[36;1m~%s\e[32;1m ᐅ \e[033;0m", path_buf + strlen(home)); // Removes the home directory from path
      }
      fflush(stdout);
      done = parse_input(input);
    }
    free(path_buf);
    free(input);
}

int parse_input(char *input) {
    char *argv = input;
    fgets(argv, BUFFER_SIZE, stdin);
    if (feof(stdin)) { // To handle EOF
        return 1;
    }
    while (argv[0] == ' ') { // To clean up spaces before commands
        argv++;
    }
    char *cmd = strsep(&argv, " \n");
    if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) {
        return 1;
    }
    else if (cmd) {
        if (strstr(cmd, "cd") != NULL) {
            change_directory(argv);
        }
        else if (strcmp(cmd, "")) { // Makes sure that the command is not empty
            printf("Running: %s %s\n", cmd, argv);
            call_cmd(cmd, argv);
        }
    }
    return 0;
}

void call_cmd(char *cmd, char *argv) {
    pid_t pid = fork();
    if (pid) { // Parent process to wait for child to finish
        int status;
        wait(&status);
    }
    else { // Child process to execute commands
        char *argv_buf[BUFFER_SIZE];
        char *arg;
        argv_buf[0] = cmd;
        int i = 1;
        for (arg = strsep(&argv, " \n"); *arg; arg = strsep(&argv, " \n")) {
            argv_buf[i] = arg;
            ++i;
        }
        argv_buf[i] = NULL;
        errno_result = execvp(cmd, argv_buf);
        if (errno_result == -1) {
            printf("%s: %s\n", cmd, strerror(errno));
        }
        exit(0);
    }
}

void change_directory(char *argv) {
    while (argv[0] == ' ') {
        argv++; // Remove empty spaces in front of path
    }
    char *path = strsep(&argv, " \n");
    char *home_cpy = strdup(getenv("HOME"));
    if (path[0] == '~') {
        path++; // Goes beyond ~
        path = strcat(home_cpy, path);
    }
    else if (path[0] == '\0') { // When no path specified, use home
        path = home_cpy;
    }
    errno_result = chdir(path);
    if (errno_result == -1) {
        printf("%s: %s\n", "cd", strerror(errno));
    }
    free(home_cpy);
}
