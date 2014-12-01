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
