#shell
##Ivy Wong and Lise Ho

##Features:
	Forks and executes commands
	Runs multiple commands on one line 
	Deals with Extra Spaces in between commands ?(i.e.  ls       -a  -l)
	      BUT ( ls   -    a   ) WILL break the shell
	Redirects with >, < (simple redirection)
	Redirects with | (simple piping)

##Attempted:
	Multiple |

##Bugs:
	Putting two ;'s next to each other causes a seg fault (this doesn't normally work on a regular terminal anyway)
	Multiple |'s in a row doesn't run all the pipes

##Files & Function Headers:
###shell.c
	/*======== char * strip(char * p) ==========
	Inputs:  char * p
      	Returns: A char array [string] with the spaces and new lines 'stripped' from the front and ends of the inputted string

	Takes out leading and trailing spaces / new lines
	====================*/	
	/*======== void parse_redirect(char * s) ==========
	Inputs: char * s
	Returns: Nothing
	
	Deals with redirection > , <
	====================*/	
	/*======== void parse_string(char * s) ==========
	Inputs: char * s
	Returns: Nothing

	Parses the command s into an argument array [for execvp] and calls exec which executes the command
	====================*/	      
	/*======== void exec(char ** argarray, int len) ==========
	Inputs: char ** argarray
		int len
	Returns: Nothing
	
	forks and execvp the commands with the exception of cd and exit which the function executes manually
	====================*/	      
	/*======== void shell() =========
	Inputs: None
	Returns: Nothing

	Asks user for the command and parses through the command for semi-colons, and redirection symbols and calls the parse commands needed
	====================*/	      
	/*======== static void sighandler(int signo) ==========
	Inputs: int signo
	Returns: Nothing

	Deals with Keyboard Interruptions - allows keyboard interruptions to exit processes run in the shell
	====================*/	      
	/*======== int main() ==========
	Inputs: None
	Returns: 0
	
	Runs the shell in a while loop
	====================*/	      
###pipes.c
	Deals with piping (did not have to be another file, but shell.c was getting to cluttered)
	/*======== void piper(char * s) ==========
	Inputs: char * s
	Returns: Nothing
	
	Pipes the stdout of one command to the stdin of another command
	Multiple pipings will not work
	Uses a temporary file to store stdout to put into stdin
	Calls remove_file to remove that file
	====================*/
	/*======== void remove_file(char *f) ==========
	Inputs: char * f
	Returns: Nothing
	
	Removes file with name f
	====================*/		
