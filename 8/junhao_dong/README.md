Shellfish
======
Junhao Dong  
Systems Period 8

Rudimentary shell program


## Basic Features
- [x] Execute multiple commands per line, separated by `;`
  - [x] Trim excess white space
  - [x] Ignore consecutive semicolons rather than returning a syntax error
- [x] Basic file redirection: `>` & `<`
- [x] Basic piping: `|`
- [ ] Checked for memory leaks (Valgrind)


## To Do
###Enhancements
------
- [x] Tilde expansion: `~` is interchangeable with user's $HOME directory
- [ ] Debug mode/flag where more obscure errors are printed (currently the default)
- [ ] Colorful prompt
- [ ] Chained piping (recursion!)
- [ ] Signal handling: `SIGINT` & `EOF`
- [ ] Wildcard `*`
- [ ] Tab completion: for files and commands
- [ ] Command history & navigation
- [ ] Directory history & navigation (`cd -`)
- [ ] More complex redirection
  - [x] `>>`
  - [ ] `<<`, `<<<`
  - [ ] `cmd > out1 flag1 flag2`
  - [ ] `cmd < infile > outfile`
  - [ ] Redirection for `STDERR` and other file descriptors 
- [ ] Background processes: `&`
- [ ] Logic operators: `&&`, `||`, `!`


###Bugs
------
- [ ] Allow redirect symbols to be adjacent to commands or flags (not separated by space)
- [ ] Allow tilde expansion to work in conjunction with redirection


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
