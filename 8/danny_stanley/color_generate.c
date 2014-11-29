/* This file is only for development purposes.
 * Running this will print out all the colors and their corresponding number */

#include <stdio.h>
#include <stdlib.h>

int main() {
    int i = 0;
    int j = 0;
    int k = 0;
    for (; j < 16; j++) {
        for (; k < 16; k++) {
            i = j * 16 + k;
            printf("\e[38;5;%dm%4d ", i, i);
        }
        k = 0;
        printf("\n");
    }
    return 0;
}
