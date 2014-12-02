char **tokenize(char*);
int count_tokens(char*);
char **split_into_lines(char*);

struct CommandSet {
        int num_tokens;
        char **tokens;
};
