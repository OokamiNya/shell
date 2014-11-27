#include "shellutil.h"

int escapeIndex = 0;
int escapeSize = ESCAPE_SIZE;
char *escape_buf = (char *) NULL;
int tildeIndex = 0;
int tildeSize = TILDE_SIZE;
char *tilde_buf = (char *) NULL;

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

/* Given the input from user, and index in input
 * Returns number of characters read
 * tilde_buf must be FREED! */
int handle_tilde(char *input, int index) {
    int initIndex = index;
    tildeIndex = 0;
    ++index; // Move past '~'
    if (input[index] == '/' || input[index] == ' ' || input[index] == '\0') { // Replace ~ with $HOME when referring to directories or nothing specified
        tilde_buf = strdup(getenv("HOME"));
        tildeIndex += strlen(tilde_buf);
    }
    else { // Replace ~user with home directory of user
        tildeSize = TILDE_SIZE;
        tilde_buf = (char *) malloc(TILDE_SIZE);
        int userSize = USER_SIZE;
        int userIndex = 0;
        char *user = (char *) malloc(USER_SIZE * sizeof(char));
        while (input[index] && input[index] != ' ' && input[index] != '/' && input[index] != ';' && input[index] != '>' && input[index] != '|') {
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
            tilde_buf[tildeIndex] = '~';
            ++tildeIndex;
        }
        int home_length = strlen(user_home);
        if (home_length + 2 >= tildeSize) {
            tildeSize += home_length;
            tilde_buf = (char *) realloc(tilde_buf, tildeSize * sizeof(char));
        }
        strcpy(tilde_buf + tildeIndex, user_home);
        tildeIndex += home_length;
    }
    return index - initIndex;
}
