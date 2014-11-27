#include "colors.h"
#include "prompt.h"

int cmd_status = 1;
int valid_input = 0;

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

/* Writes current time given a buffer */
char * get_time(char *time_buf, int time_size) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char *ampm;
    int hour = tm.tm_hour;
    if (hour < 12) {
        ampm = "AM";
    }
    else {
        ampm = "PM";
        hour -= 12;
    }
    snprintf(time_buf, time_size, "[%02d:%02d:%02d %s]", hour, tm.tm_min, tm.tm_sec, ampm);
    return time_buf;
}

/* Writes prompt given a buffer */
char * create_prompt(char *prompt_buf, int prompt_size) {
    char path[PATH_SIZE];
    get_path(path, PATH_SIZE);
    char time[TIME_SIZE];
    get_time(time, TIME_SIZE);
    const char *status;
    if (cmd_status) { // Previous command was successful
        status = fg_green;
    }
    else {
        status = fg_red;
    }
    snprintf(prompt_buf, prompt_size, "%s%s%s$ %s%s%s %s%s%s %s%s\nᐅ %s", bold_prefix, fg_pink, shell_name, bold_prefix, fg_orange, time, bold_prefix, fg_cyan, path, normal_prefix, status, reset);
    return prompt_buf;
}


