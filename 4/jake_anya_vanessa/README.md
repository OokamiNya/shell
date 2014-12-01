What our shell does:

-Runs all basic commands (ls, cat, emacs...) with any number of 
arguments
-Handles cd
-Handles exit
-Swaps home-directory with "~"
-Handles pipe
-Handles semicolon
-Ignores input with typos or gibberish


What our shell was supposed to do:

-Handle tab-completion using "char * my_fgets(...)" in 
executor.c
-Handle SIGINTs using "sighandler" 


Bugs and other issues to consider:

None known.


Function headers:

static void sighandler(int signo)
What it does: Attempts to handle the SIGINT signal by exiting
if it's a child processes, and printing a newline otherwise.

int main()
What it does: Exactly what you'd think: runs the shell program.

int execute(char ** arg)
What it does: Takes the information typed in by the user as 
char ** arg and checks for semicolons and pipes and parses the
string accordingly and then calls execute on the new parsed 
strings individually. Also takes care of cases in which cd and 
exit are called. For any cases without cd, exit, semicolons,
or pipes it calls executef() on arg.

int executef(char ** arg)
What it does: Forks of a child process and executes arg using
execvp().

char *replace_string(char *str, char *old, char *rep)
What it does: Takes a string, char *str, and returns a new copy
of that string, but with the first occurrence of char *old
replaced with char *rep.

char *my_fgets(char *s, int size, FILE *stream)
What it does: Attempts to copy the functionality of fgets, but
separating on not only newlines but tabs as well. Additionally
it would have contained tab-completion code within it.