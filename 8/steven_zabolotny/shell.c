#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/* char *pre()
   Inputs: None
   Returns: The entire directory you are currently in.

   This function is called pre because it gives you the directory that appears before you enter commands.
   It also includes the name of the shell, Swag Shell. */
char *pre() {
  char cwd[256];
  getcwd(cwd, 256);
  printf("Swag Shell:%s$ ",cwd);
}

/* void *cd()
   Inputs: char *input
   Returns: Nothing

   A special function to change the directory of the shell. This cannot be used in run() because cd does not work in a forked process. 
   It takes the input and the directory you want to go to and chdirs to it. Works with cd no arguments and cd with ... */
void *cd(char *input) { // Runs the cd command
  //printf("<%s>\n",strchr(input,' '));
  if (strlen(input) <= 4) { // Check that no args are given
    chdir("/");
  } else {
    char cwd[256];
    getcwd(cwd, 256);
    char *dest = strchr(input,' ');
    dest[0] = '/';
    dest[strlen(dest) - 1] = '\0';
    //printf("Chdiring to <%s>",strcat(cwd, dest));
    chdir(strcat(cwd, dest)); // Moves you into a directory in your current directory
  }
}

/* void *run()
   Input: char *input
   Returns: Nothing

   Given the user input processed by main(), it splits up the command by spaces and then forks a process that is used to run these 
   commands with the appropriate arguments. 
   This function is multi-purpose and used to run any commands without pipes, redirection, or semicolons. 
   This is the only function that actually runs commands. */
void *run(char *input) { // Runs any non-cd command
  //printf("<%s>\n",input);
  int status;
  int f = fork ();
  char *args[256];
  int i = 0;
  char *tok = input;
  char *end = input;
  while (tok != NULL) { // The space added earlier is necessary here to make strsep work on the last arg properly.
    strsep(&end, " ");
    args[i] = tok;
    i++;
    tok = end;
  }
  args[i - 1] = NULL; // Null-terminates the array and removes the last empty arg
  if (f) {
    wait(&status);
  } else {
    //printf("<%s>\n",args[0]);
    execvp(args[0], args); // Useful for large amounts of arguments
  }
}

/* void *multi()
   Input: char *input (containing at least one ;)
   Returns: Nothing

   Given the user input processed by main(), it splits up the command by semicolons, finding each seperate command-argument block. 
   Each command-argument block is then run and executed. 
   Does not actually execute commands itself, utilizes run() for this purpose. */
void *multi(char *input) {
  int a = strlen(input);
  input[a] = ';';
  //input[a + 1] = ' ';
  input[a + 1] = '\0';
  int i = 0;
  char *tok = input;
  char *end = input;
  //printf("<%s>\n",input);
  while (tok != NULL) {
    strsep(&end, ";");
    /*int b = strlen(tok);
      if (tok[b - 1] != ' ') {
      tok[b] = ' ';
      tok[b + 1] = '\0';
      }*/
    int b;
    if (tok[0] == ' ') { // Get rid of those pesky spaces
      for (b = 0;b < strlen(tok);b++) {
	tok[b] = tok[b + 1];
      }
    }
    //printf("<%s><%d>\n",tok,strlen(tok));
    if (tok[0] == 'c' && tok[1] == 'd') // In case you're cding
      cd(tok);
    else if (tok != NULL && strlen(tok) > 0) // This fixes space issues
      run(tok);
    i++;
    tok = end;
  }
}

/* void *redirect()
   Input: char *input (containing at least one > or <), char *r (either < or >)
   Returns: Nothing

   Given the user input processed by main() and the type of redirection determined to be in this command by main, it splits up the command by > or <. 
   Then, either stdout or stdin are redirected appropriatly, and then the command is run and executed. 
   Finally, stdout and stdin are restored to their original values.
   Does not actually execute commands itself, utilizes run() for this purpose. */
void *redirect(char *input, char *r) {
  char *args[256];
  int i = 0;
  char *tok = input;
  char *end = input;
  while (tok != NULL) {
    strsep(&end, r);
    int b;
    if (tok[0] == ' ') { // Get rid of those pesky spaces
      for (b = 0;b < strlen(tok);b++) {
	tok[b] = tok[b + 1];
      }
    }
    args[i] = tok;
    i++;
    tok = end;
  }
  
  args[1][strlen(args[1]) -1] = '\0'; // Get rid of space at end
  int status;
  int f = fork ();
  if (f) {
    wait(&status);
  } else {
    int fd = open(args[1], O_RDWR | O_CREAT | O_APPEND, 0644); // Opens the file being redirected to
    if (r == ">") {
      int saved_stdout = dup(1);
      dup2(fd, 1); // Redirects stdout to the file
      close(fd);
      //printf("<%s>\n",args[0]);
      run(args[0]);
      dup2(saved_stdout, 1);
      close(saved_stdout);
    } else if (r == "<") {
      int saved_stdin = dup(0);
      dup2(fd, 0); // Redirects stdin to the file
      close(fd);
      //printf("<%s>\n",args[0]);
      run(args[0]);
      dup2(saved_stdin, 0);
      close(saved_stdin);
    }
  } 
}

/* void *piper()
   Input: char *input (containing at least one |)
   Returns: Nothing

   Given the user input procesed by main(), it splits up the command by |. 
   Then, the first command is run and it's output is redirected to a temporary file, and the temporary file is redirected as the input 
   for the second command, which is also run. Finally, the temporary file is removed.
   Does not actually execute commands itself, utilizes run() and redirect() for this purpose. */
void *piper(char *input) { // Doesn't work yet
  char *args[256];
  int i = 0;
  char *tok = input;
  char *end = input;
  while (tok != NULL) {
    strsep(&end, "|");
    int b;
    if (tok[0] == ' ') { // Get rid of those pesky spaces
      for (b = 0;b < strlen(tok);b++) {
	tok[b] = tok[b + 1];
      }
    }
    args[i] = tok;
    i++;
    tok = end;
  }
  //int a;
  //for (a = 0; a < i; a++) {
  //  args[a][strlen(args[1]) -1] = '\0'; // Get rid of space at end
  //}
  
  int fd = open("temp.txt", O_RDWR | O_CREAT | O_APPEND, 0644); // Opens the file being redirected to

  int saved_stdout = dup(1);
  dup2(fd, 1); // Redirects stdout to the file
  //printf("<%s>\n",args[0]);
  run(args[0]);
  dup2(saved_stdout, 1);
  close(saved_stdout);

  // I don't know why I can't run redirect the first time, but it works
  args[1][strlen(args[1]) - 1] = '\0';
  redirect(strcat(args[1]," < temp.txt "),"<");

  char rm[256] = "rm temp.txt ";
  run(rm);
  close(fd);
}

/* int main()
   Input: None
   Returns: Everything

   Runs a permanent while loop which keeps the shell running unless the process is killed. It also interprets input, 
   adds a space at the end which is necessary for run(), and decides which of the above functions is needed to be run on the input 
   based on the presence of certain characters (;,<,>,|).
   Also interprets cd and exit, which change directory and exit the shell. */
int main() {
  pre();
  while(1) {
    //This processes the input
    char input[256];
    fgets(input, 256, stdin);
    int a;
    for (a = 0;a < strlen(input);a++) { // Adds a space before the end of the buffer
      if (input[a] == '\n') {
	input[a] = ' ';
	break;
      }
    }

    if (input[0] == 'e' && input[1] == 'x' && input[2] == 'i' && input[3] == 't') {
      //execlp("exit", "exit", NULL);
      return 0;
    } else if (input[0] == 'c' && input[1] == 'd') {
      cd(input);
      pre();
    } else {
      if (strchr(input, ';') != NULL) {
	multi(input);
      } else if (strchr(input, '>') != NULL) {
	redirect(input, ">");
      } else if (strchr(input, '<') != NULL) {
	redirect(input, "<");
      } else if (strchr(input, '|') != NULL) {
	piper(input);
      } else {
	run(input);
      }
      pre();
    }
  }
  return 0;
}
