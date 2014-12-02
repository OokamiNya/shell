#include <stdio.h>

int main(int argc, char *argv[]) {
	char file_input[256];
    fgets(file_input, sizeof(file_input), stdin);
    printf("     RUNNING FROM TEST.C\n");
    printf("     INPUT %s\n", file_input);
    return 0;
}
