(╯°□°)╯︵ Shell ︵ ╯(°□° ╯)
=====
Barak Zhou

System Level Programming pd. 8

Shell is exactly what it is: A simple shell.
Concisely built. (For more than one reason...)

Type a command. Run a program.
Easily compiled with a makefile.
Exit with "exit" or a keyboard interrupt (^C).

##Features
- Execute terminal commands.
- Chain multiple commands with ;.
- Redirect files with > or < (use only one per chain segment).
- Basic Piping with | (use only one per chain segment).
- Simple structure means easy to keep track of memory.

##Unsuccessful Features
- Rest in peace project v1 2014-11-22 -- 2014-11-22: You will be missed. git push regularly!
 - Lost chain redirection (but not chain piping)
 - Lost the ~ as home directory; new implementation was unworthy of presentation
- Attempted to chain redirection and piping recursively using a system of pipes (the method pipe()). After several seg faults, memory overloads, hours wasted/spent crying, and some research, I realized that this was a project to be tackled on its own.

##Bugs
- Sometimes "exit" will just reset the shell. I don't know why.
- If tab or any arrow keys are pressed, the corresponding input will be entered. Not really a bug, but an inconvenience.
- git commit: the command "git commit -am "string here"" parses the space inside the string, so a commit through the shell cannot have spaces in the string.

##Files
- shell.h and shell.c: plain and simple.
- makefile

##Functions
```
/**
 * int execute ( char** input )
 * ============================
 *
 * Takes an array of strings and executes them as a command,
 * with input[0] as the command and other strings as parameters
 *
 * Parameters:
 *     char** input: Array of strings
 *
 * Return:
 *     0 if successful.
 *
 * -Note-: this return was supposed to be used for a recursive
 * redirect function, but it was never implemented.
 *
 **/
```

```
/**
 * char** parse ( char* input, char* delim )
 * =========================================
 *
 * Takes a string and parse it by the delimiter.
 * 
 * Parameters:
 *     char* input: String to be parsed.
 *     char* delim: Delimiter.
 *
 * Return:
 *     A pointer to an array of strings after parse, 
 *     with memory allocated.
 *
 **/
```

```
/**
 * char* find_redirect ( char* input )
 * ===================================
 *
 * Takes a string and finds the first occurence of >, < or |
 * 
 * Parameters:
 *     char* input: String to be analyzed.
 *
 * Return:
 *     A pointer to the first char >, < or | found in input.
 *
 * -Note-: use return[0] to get the actual char.
 *     
 **/
```

```
/**
 * int main()
 * ==========
 *
 * Main method.
 *
 * Process of input/output:
 *     1. Get the console input
 *     2. Parse by ; and execute individual commands
 *     3. Check: Is there a redirect in there?
 *         Yes:
 *             a. Check which redirect
 *             b. Parse by space and execute accordingly.
 *         No:
 *             a. Execute accordingly.
 *     4. Free the memory.
 *     5. Repeat.
 *
 **/
```
