void printPrompt();
void redirect();
void execute(char **argv);
// Free dynamically allocated memory after use
char ** parseInput(char *input, char *tok);
void shell();
