//int main()
/*
Returns 0 for no reason whatsoever.

In the infinite loop, an array of strings (char ** args) is allocated to hold user input (see parse() below) and handle said user input (see execute() below).  Since fgets() is called in parse(), the loop will pause and wait for user input each time through.
 */

static void sighandler(int signo);
/*
It was a meek attempt to handle keyboard interrupts.
It would work the first time but thereafter would erupt.
(kinda)
(it just failed to cancel a subprocess after more than one Ctrl+C)
(and then there was no way to escape)

**UPDATE** (sorry I didn't delete the above part, I just didn't want to abandon the poetic embellishment)
This function basically works now, because it's called _inside_ the while loop in main().  It doesn't do much.  It just prevents a keyboard interrupt from closing our shell entirely and returning to bash.  But since it _really_ doesn't do much, you have to hit Enter/Return to satisfy fgets() as called in parse() in order to regain the prompt.
 */

void print_prompt();
/*
Self-explanatory.
Prints current working directory.
Called at the end of the while loop in main().
 */

void print_array(char ** a);
/*
Also self-explanatory.
Prints an array of strings, as given by the parameter.
We used it for testing purposes only.  It is not called anywhere in the program.
 */

void parse(char ** args);
/*
Uses fgets() to parse user input from the command-line prompt into an array of strings.

The parameter is used as a buffer: 'args' must have space allocated to hold the array of strings assigned to it inside parse().  (The only time parse() is called is in main(), and the char ** args is allocated before it is used as a buffer via parse().)

_Under the condition_ that each element of the user input is separated by one or more spaces (' '), parse() will translate the user input from a single string into an array of strings using two pointers and a while loop.  After the first call of strsep(), 'temp' will point to the first element in the user input, and 's' will point to the second.  Each pass through the while loop, the string pointed to by 'temp' will be stored in 'args', and then both 'temp' and 's' will move forward one element in the line of user input.  After the while loop is completed and all arguments have been stored in 'args', 'args' is terminated with NULL.
 */

int contains(char ** args, char * c);
/*
This function is used as a helper function in execute() (see below) to look for the special characters ';', '<', '>', and '|', as well as the keywords 'cd' and 'exit'.

Checks if the string 'c' is contained in the array of strings 'args'.
If 'c' is contained in 'args', contains() will return the index of the first occurence of 'c' in 'args'.
If 'c' is _not_ contained in 'args', contains() will return -1.
 */

void redirect(int type, int i, char ** args);
/*
Called by execute() when '<', '>', or '>>' is detected in its parameter args.

redirect() takes a type number (which is used only in execute()) as a shorthand way of representing the type of redirection (input, output-overwrite, or output-append) being invoked; the array of strings args which contains the commands to be handled; and the index i at which the special character resides in args.

A child process will be forked for that sake of executing a command.
In the child process, the file indicated by the user (the filename given after '<', '>', or '>>') will be opened or created.  If '<' is used, standard input (STDIN) will be pulled from said file; if '>' or '>>' is used, standard output (STDOUT) will be written into said file.  Both those cases are handled by a simple call of dup2() (see the Manual page entry for more information on dup2()).
The commands given before the redirection symbol ('<', '>', or '>>') are copied into a new array of strings, char ** part1, so that they may be run via execvp().  When the commands are run, they will--respectively--take their input OR send their output to the file indicated by the user.
If, for some reason, execvp() fails to run, the child process will be terminated using kill().

The parent process will wait for its child process to to finish before it resumes.
*/

int execute(char ** args);
/*

 */
