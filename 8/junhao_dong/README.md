Shellfish
======
Junhao Dong  
Systems Period 8

Rudimentary shell program

## Features
- [x] Tilde expansion: `~` is replaced by user's $HOME directory
- [x] Executes multiple commands per line, separated by `;`
  - [x] Trims excess white space and semicolons
  - [x] Ignores consecutive semicolons rather than returning a syntax error
- [x] Basic file redirection: `>` & `<`
  - [x] All args following a redirect symbol, excluding the first, are ignored
- [x] Pipes: `|`

## TODO (in no particular order)
- [ ] Colorful prompt
- [ ] Signal handling: `SIGINT` & `EOF`
- [ ] Wildcard `*`
- [ ] Tab completion
- [ ] Command history & navigation
- [ ] Directory history & navigation (`cd -`)
- [ ] More complex redirection
  - [ ] `>>`, `<<`, `<<<`
  - [ ] `cmd < infile > outfile`
  - [ ] Redirection for `STDERR` and other file descriptors 
- [ ] Background processes
- [ ] Interpreter

## Files & Function Headers
shell.c

== DW'S things for reference; will delete later ==

Handles all line parsing fucntions
======== int count_tokens() ==========
Inputs:  char *line
char delim 
Returns: Number of tokens in line separated by delim

Counts the number of times the character delim appears in the
string line
The number of tokens is 1 more than the number of appearences 
of delim
If delim does not appear, 1 is returned

======== char ** parse_line() ==========
Inputs:  char *line 
Returns: Array of strings where each entry is a token 
separated by delim

If line contains multiple tokens separated by delim, this 
function will put each token into an array of strings

======== char * trim() ==========
Inputs:  char *line 
Returns: Pointer to the beginning of line

Removes leading and trailing whitespace on the string line.
Terminating '\0' is placed at a new location if necessary.
