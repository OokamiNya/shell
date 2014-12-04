
1.) features
	-Forks and executes commands
	-Parses multiple commands on one line
	-Redirects using > , >> , <
	-Pipes (one per command)
	-parse works with more than one space
	-cd use special character ‘~’ when cd-ing


2.)attempted (to varying extents)
	-there must be at least one space between special characters (; ,>, >>, <, |)
	-sig handling ^C only kinda sorta works
	 (left in code, will exit current command but won’t print prompt again)
	
3.)bugs
	-when gibberish is entered it doesn’t come up as an error
	(but now it prints a line, then just prints the prompt again)
	-Freeing! what runs on my mac isn’t running on Mary’s ubuntu, and what’s running on Mar’s ubuntu isn’t running on my mac! so we just commented out freeing in main, and left this comment there too if you want to test it out

FUNCTION HEADERS

/*=========================== int main() ===========================
Returns 0 for no reason whatsoever.

In the infinite loop:
- fgets() is called to get user input
- an array of strings (char ** args) is allocated to hold user input
- the user input is parsed (using spaces as delimiters) into separate strings (see parse() below)
and
- said user input is handled/executed (see execute() below)
 ======================================================*/


/*===========================static void sighandler(int signo);===========================
/*
It was a meek attempt to handle keyboard interrupts.
It would work the first time but thereafter would erupt.
(kinda)
(it just failed to cancel a subprocess after more than one Ctrl+C)
(and then there was no way to escape)

**UPDATE** (sorry I didn't delete the above part, I just didn't want to abandon the poetic embellishment)
This function basically works now, because it's called _inside_ the while loop in main().  It doesn't do much.  It just prevents a keyboard interrupt from closing our shell entirely and returning to bash.  But since it _really_ doesn't do much, you have to hit Enter/Return to satisfy fgets() as called in parse() in order to regain the prompt.
====================================================== */


/*===========================int count_chars(char * s, char c);===========================
Returns the number of times char 'c' appears in string 's'.
======================================================*/


/*===========================void print_prompt();===========================
Self-explanatory.
Prints current working directory.
Called at the end of the while loop in main().
====================================================== */


/*===========================void print_array(char ** a);===========================
Also self-explanatory.
Prints an array of strings, as given by the parameter.
Used it for testing purposes only.  It is not called anywhere in the program.
 ======================================================*/


/*========================void parse(char ** args, char * s1);===========================
input:
- args is pre-allocated char **
- s1 is string of user input

The parameter is used as a buffer: 'args' must have space allocated to hold the array of strings assigned to it inside parse().  (The only time parse() is called is in main(), and the char ** args is allocated before it is used as a buffer via parse().)

_Under the condition_ that each element of the user input is separated by one or more spaces (' '), parse() will translate the user input from a single string into an array of strings using two pointers and a while loop.  After the first call of strsep(), 'temp' will point to the first element in the user input, and 's' will point to the second.  Each pass through the while loop, the string pointed to by 'temp' will be stored in 'args', and then both 'temp' and 's' will move forward one element in the line of user input.  After the while loop is completed and all arguments have been stored in 'args', 'args' is terminated with NULL.
 ======================================================*/


/*======================int contains(char ** args, char * c);======================

This function is used as a helper function in execute() (see below) to look for the special characters ';', '<', '>', and '|', as well as the keywords 'cd' and 'exit'.

Checks if the string 'c' is contained in the array of strings 'args'.
If 'c' is contained in 'args', contains() will return the index of the first occurence of 'c' in 'args'.
If 'c' is _not_ contained in 'args', contains() will return -1.
 ======================================================*/


/*===================void redirect(int type, int i, char ** args);=======================
Called by execute() when '<', '>', or '>>' is detected in its parameter args.

redirect() takes a type number (which is used only in execute()) as a shorthand way of representing the type of redirection (input, output-overwrite, or output-append) being invoked; the array of strings args which contains the commands to be handled; and the index i at which the special character resides in args.
Note that redirect() only allows for single redirections.  Yes, this is quite unfortunate.

A child process will be forked for that sake of executing a command.
In the child process, the file indicated by the user (the filename given after '<', '>', or '>>') will be opened or created.  If '<' is used, standard input (STDIN) will be pulled from said file; if '>' or '>>' is used, standard output (STDOUT) will be written into said file.  Both those cases are handled by a simple call of dup2() (see the Manual page entry for more information on dup2()).
The commands given before the redirection symbol ('<', '>', or '>>') are copied into a new array of strings, char ** part1, so that they may be run via execvp().  When the commands are run, they will--respectively--take their input OR send their output to the file indicated by the user.
If, for some reason, execvp() fails to run, the child process will be terminated using kill().

The parent process will wait for its child process to to finish before it resumes.
======================================================*/


/*===========================int execute(char ** args);===========================
execute() is the function which executes.
It is called in main(), and, in certain cases, recursively.

It, like main(), returns 0 for no reason whatsoever.

The parameter args is an array of strings containing the parsed input entered by the user.  The 'args' used as input for execute() in main() is the same string array 'args' used as input for parse() in main().  Thus 'args' is filled up by parse() before being sent to execute().

All of the code in execute() is conditional (if, else if, ..., else).

The first condition checked is whether 'args' contains a semicolon.  If so, the elements before the semicolon will be copied into a new array of strings, char ** part1; the pointer 'args' will be moved forward to point to the first element _after_ the semicolon; and then 'part1' and the updated 'args' will be run through execute() recursively.
By checking for semicolons before any other condition, no line of commands will be executed until it has been recursively split into individual pieces.

The next three conditions deal with non-pipe redirection.  Each calls redirect() accordingly (see redirect() above).

The next condition deals with pipe redirection.  (Like non-pipe redirection, this is limited to a single pipe.  We extend our most sincere apologies and deepest condolences for our shortcoming and the inconvenience and pain you may subsequently suffer.)
Here's how the piping here works:
- The index of the pipe is stored in 'i' by using the contains() function (see contains() above).
- The commands before the pipe are copied into the string array 'part1'.
- At the end of 'part1' is appended [">"]["buffer.txt"].
- The commands after the pipe are copied into the string array 'part2'.
- At the end of 'part2' is appended ["<"]["buffer.txt"].
- 'part1' is executed, storing the output of the commands before the pipe in buffer.txt.
- 'part2' is executed, using the data in buffer.txt as the input of the commands after the pipe.
- 'part1' and 'part2' are freed, and "buffer.txt" is deleted.

The next condition checks for a "cd" call.  chdir() will change the shell's directory (see the Manual page entry for more information on chdir()).

The n-1 condition checks for an "exit" call.  The shell will close, and you will be sent back to bash.

The n condition (i.e. "else") will fork a child to execute the commands stored in 'args'.  In the case that execvp() is not called, you will get a fun message, and the child process will be terminated using kill().  The parent will resume after execvp() has usurped the child or the child has been terminated.
 ======================================================*/


//Alleluia.
