StD
---

A modern shell built by modern people, featuring most of the commands that you will ever need.

Made with ♡ by Stanley Lok and Danny Qiu

## Requirements

- Readline Library (http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html)

## Running

    $ make

For debugging:

    $ make debug

For automated testing:

    $ make test

## Features:

- Colorful Prompt
    - Current time
    - Current directory
    - `ᐅ` changes color based on success/failure of previous command
- Builtin commands:
    - `cd`, `cd -` to backtrack
    - `exit/quit`
- Home directory recognition:
    - `~` expands to home directory of environment
    - `~user` expands to home directory of user specified
- Character escapes:
    - `\`, `'`, `"`
- Multiple commands:
    - `;` allows chaining of commands
- File redirection:
    - `>`, `>>` output redirection
    - `<` input redirection
- Piping:
    - `|` allows chaining of pipes
- Handler for signals:
    - `SIGINT` generates a new prompt
    - `SIGQUIT` dings the terminal
- Readline library features:
    - Tab Completion
    - History (Using the arrow keys)
- Background Processes (VERY BAD)
    - `jobs` gives a list of all background processes whether they are currently running or not

## Documentation

Explanation of each file and function can be found [here](https://github.com/mks65-dw/shell/blob/master/8/danny_stanley/DESIGN.md).

## TODO: (We didn't have enough time to implement these ☹)

- Clear input buffer when handling SIGINT
- Wildcard handling: `*`
- Replace !! with previous function
- Smarter tab completion
- Setting variable values
- Math
- Control Statements: `for`, `if`, `while`
- _Create super awesome functions_

## What we dream of doing

- Shipping our shell
