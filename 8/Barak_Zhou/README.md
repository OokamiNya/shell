Shell
(╯°□°)╯︵ ┻━┻ ︵ ╯(°□° ╯)
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

##Functions
-test
