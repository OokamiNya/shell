#include <stdlib.h>
#include <string.h>

// Read a string in memory and split it into tokens.
//
// Returns: An array of pointers to tokens.
char **tokenize(char *str) {
        const char *delimiters = " ";

        char *copy = strdup(str);
        char **tokens = malloc(sizeof(char*) * strlen(copy));

        char *token;
        int i = 0;
        while ((token = strsep(&copy, delimiters)) != NULL) {
                tokens[i++] = token;
        }
        free(copy);

        return tokens;
}

int count_tokens(char *str) {
        if (strlen(str) == 0)
                return 0;

        const char delimiter = ' ';
        int n = 1;
        char *target = strchr(str, delimiter);
        while (target != NULL) {
                n++;
                target = strchr(target+1, delimiter);
        }
        return n;
}

char **split_into_lines(char *line) {
}
