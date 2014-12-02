#include <stdbool.h>
#include <stdio.h>

#include "ui.h"

void print_prompt() {
        printf("jksh$ ");
}

void get_line(char *buffer, int bufsize) {
        print_prompt();

        char blacklist[] = {'\n', EOF};
        int i = 0;
        char c;
        while (i < bufsize - 1) {
                c = getchar();
                if (is_blacklisted(c, blacklist))
                        break;
                buffer[i++] = c;
        }

        if (c == EOF && feof(stdin)) {
                printf("EOF encountered.\n");
                clearerr(stdin);
        }

        if (i == bufsize - 1)
                printf("Buffer size exceeded. Maximum %i characters allowed.\n", bufsize - 1);
        else
                buffer[i] = 0;
}

int is_blacklisted(char c, char *blacklist) {
        int len = sizeof(blacklist);
        int i;
        for (i = 0; i < len; i++) {
                if (c == blacklist[i])
                        return true;
        }

        return false;
}
