What our shell does:

-Runs all basic commands (ls, cat, emacs...) with any number of arguments
-Handles cd
-Handles exit
-Swaps home-directory with "~"
-Handles pipe
-Handles semicolon
-Ignores input with typos or gibberish


What our shell was supposed to do:

-Handle tab-completion using "char * my_fgets(...)" in executor.c
-Handle SIGINTs using "sighandler" 


Bugs and other issues to consider:

None known.


Function headers:

/*------------------------------------------------------------
int execute()
Input: char ** arg
What it does: Takes the information typed in by the user as 
char ** arg and checks for semicolons and pipes and parses the
string accordingly and then calls execute on the new parsed 
strings individually. Also takes care of cases in which cd and 
exit are called. For any cases without cd, exit, semicolons,
or pipes it calls executef() on arg.
------------------------------------------------------------*/

/*------------------------------------------------------------
int executef()
Input: char** arg
What it does: Forks of a child process and executes arg using
execvp().
------------------------------------------------------------*/
