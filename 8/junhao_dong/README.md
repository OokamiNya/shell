Shellfish
by Junhao Dong

==COPIED FROM DW'S EXAMPLE, WILL BE CUSTOMIZED LATER==
Features:
	Forks and executes commands!
	Parses multiple commands on one line!
	Redirects using >, <!

Attempted:
	Could not get pipes to work
	Tried to implement >>, but it kept overwriting the file
	Looked at replacing ~/ with home directory, got seg faults

Bugs:
	Redirecting to a file that does not exist occasionally
	does not work.

Files & Function Headers:
parse.c
	Handles all line parsing fucntions
	/*======== int count_tokens() ==========
	Inputs:  char *line
        	   char delim 
		   Returns: Number of tokens in line separated by delim

		   Counts the number of times the character delim appears in the
		    string line
		    The number of tokens is 1 more than the number of appearences 
		    of delim
		    If delim does not appear, 1 is returned
		    ====================*/

		    /*======== char ** parse_line() ==========
		    Inputs:  char *line 
		    Returns: Array of strings where each entry is a token 
		    separated by delim

		    If line contains multiple tokens separated by delim, this 
		    function will put each token into an array of strings
		    ====================*/

		    /*======== char * trim() ==========
		    Inputs:  char *line 
		    Returns: Pointer to the beginning of line

		    Removes leading and trailing whitespace on the string line.
		    Terminating '\0' is placed at a new location if necessary.
		    ====================*/

dwsh.c
	Handles the forking an executing of commands...
