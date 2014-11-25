#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

char *pre() {
  char cwd[256];
  getcwd(cwd, 256);
  printf("Swag Shell:%s$ ",cwd);
}

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

void *run(char *input) { // Runs any non-cd command
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
      execvp(args[0], args); // Useful for large amounts of arguments
    }
}

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
	run(args[0]);
	dup2(saved_stdout, 1);
	close(saved_stdout);
      } else if (r == "<") {
	int saved_stdin = dup(0);
	dup2(fd, 0); // Redirects stdin to the file
	close(fd);
	run(args[0]);
	dup2(saved_stdin, 0);
	close(saved_stdin);
      }
    } 
}

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
    int a;
    for (a = 0; a < i; a++) {
      args[a][strlen(args[1]) -1] = '\0'; // Get rid of space at end
    }
    
    redirect(strcat(args[0], " > temp.txt"), ">");
    redirect(strcat(args[1], " < temp.txt"), "<");
    run("rm temp.txt");
}

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
