
//runs our basic commands with execvp and arrays of arguments
void parse(char *input);

//handles the special case of cd
void mycd (char** commands, int numargs);

//handles redirection before calling parse to run command
void redirect(char *input);

//checks for piping, breaks up pipes into separate functions, and sends to mypipe to run
//calls redirect if piping is not used
void process(char *input);

//handles piping of commands using temporary files to store intermediate input/output
void mypipe(char **commands, int numArgs);
