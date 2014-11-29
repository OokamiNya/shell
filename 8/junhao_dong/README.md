Shellfish
======
Junhao Dong  
Systems Programming, Period 8

Shell program


## Basic Features
- [x] Execute multiple commands per line, separated by `;`
  - [x] Trim excess white space
  - [x] Ignore consecutive semicolons rather than returning a syntax error
- [x] Basic file redirection: `>` & `<`
- [x] Basic piping: `|`
- [x] Checked for memory leaks: Valgrind


## To Do
###Enhancements
------
- [x] Tilde expansion: `~` is interchangeable with user's $HOME directory
- [x] Prompt: username, current working directory
  - [ ] Color
- [x] Better redirection: `>>`
  - [x] `cmd < inFile > outFile`
  - [x] `cmd > out1 flag1 flag2`
  - [x] Work with pipes
  - [ ] `<<`, `<<<`
  - [ ] Redirection for `STDERR` and other file descriptors 
- [x] Chained piping
- [ ] Wildcard `*`
- [ ] Signal handling: `SIGINT`
  - [ ] `EOF` (Ctrl-D)
- [ ] Tab completion: for files and commands
- [ ] Command history & navigation
- [ ] Directory history & navigation (`cd -`)
- [ ] Logic operators: `&&`, `||`, `!`
- [ ] Background processes: `&`, Ctrl-Z


###Bugs to be fixed
------
- [ ] Allow redirect and pipe symbols to be adjacent to commands or flags (not separated by space)
- [ ] Allow tilde expansion to work in conjunction with redirection


## Function Headers
shell.h

- static void sigHandler(int signo)
- void printPrompt()
  - Prints the shell prompt with the current working directory, replacing $HOME with ~ when applicable
- void safe_exec()
- char * trimSpace(char *str)
- void redirect()
- void executePipe(int pipeIndex)
- void executeMisc()
- void execute()
- char ** parseInput(char *input, char *delim)
- void shell()


Ignore==== (here for reference; taken from DW's example)

char ** parse_line()
Returns: Array of strings where each entry is a token 
separated by delim

If line contains multiple tokens separated by delim, this 
function will put each token into an array of strings

char * trim()
Returns: Pointer to the beginning of line

Removes leading and trailing whitespace on the string line.
Terminating '\0' is placed at a new location if necessary.
