/**
 * Shell.c
 * Contains main()
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int execute(char** input) {
  if (!strcmp(input[0], "cd")){
    if (!input[1]) {
      chdir(getenv("HOME"));
    }
    else if (chdir(input[1]) < 0) {
      printf("cd: %s: %s\n", input[1], strerror(errno));
    }
  }
  else if (!strcmp(input[0], "exit")){
    exit(EXIT_SUCCESS);
  }
  else {
    // getting there
    int spoon = fork();
    int status;
    if (!spoon) {
      execvp(input[0],input);
    }
    else {
      wait(&status);
    }
  }
  return 0;
}

char** parse ( char* input, char* delim ) {
  strtok(input, "\n"); //this guy again...

  int i = 0;
  char* str = input;
  while(*str){
    if(*str == *delim){
      i++;
    }
    str++;
  }
  char* arg = strtok(input, delim);
  //somehow this works
  //printf("%d\n", i+2);
  char** argv = calloc(i+2,sizeof(char *));
  argv[0] = arg;
  int j = 0;
  while(arg){
    j++;
    arg = strtok(NULL, delim);
    argv[j] = arg;
  }

  
  //print everything - use with caution
  /*
  int k = 0;
  for (;k<i+2; k++) {
    printf("argv[%d]: %s\n", k, argv[k]);
  }
  */

  return argv;
}

//returns the first location of a redirect, but don't print it! use return[0]
char* find_redirect( char* input ) {
  char* redirect_stdin = "<";
  char* redirect_stdout = ">";
  char* pipe = "|";
  while (*input) {
    if (*input == *redirect_stdin || *input == *redirect_stdout || *input == *pipe) {
      return input;
    }
    input ++;
  }
  return 0;
}


int main() {
  char cwd[256];
  int running = 1;

  char input[256];
  while (running) {
    char** args = 0;
    char** argv = 0;
    char** arg = 0; //at this point I forgot what the names mean and I'm making these up

    printf("(╯'□')╯%s: ", getcwd(cwd, sizeof(cwd)));
    
    fgets(input, sizeof(input), stdin);
    
    //printf("Unparsed:\n");
    args = parse(input,";"); //args has a bunch of commands unparsed by space, parsed by ;
    //printf("Parsed:\n");

    int i = 0;
    while (args[i]) { //execute every command, parsing the > < | as well
      
      //char* redir = find_redirect(args[i]);
      //printf("%c\n", redir[0]);
      
      //split the string at the redirect
      char* redir;
      if (redir = find_redirect(args[i])) {

	//if redirecting stdout
	if (redir[0] == '>') {
	  arg = parse(args[i],">"); //bad things happen if nothing comes after >!!!!!!!!!!!
	  argv = parse(arg[0]," ");
	  char* filename = parse(arg[1]," ")[0]; //take the space out of the name
	  int spoon = fork();
	  int status;
	  if (!spoon) {
	    int fd = open(filename, O_CREAT | O_WRONLY, 0777);
	    int temp_stdout = dup(STDOUT_FILENO);
	    dup2(fd, STDOUT_FILENO);
	    execute(argv);
	    dup2(temp_stdout, STDOUT_FILENO);
	  }
	  else {
	    wait(&status);
	  }
	} //end stdout
	//if redirecting stdin
	else if (redir[0] == '<') {
	  arg = parse(args[i],"<"); 
	  argv = parse(arg[0]," ");
	  char* filename = parse(arg[1]," ")[0]; //take the space out of the name
	  int spoon = fork();
	  int status;
	  if (!spoon) {
	    int fd = open(filename, O_RDWR, 0777);
	    int temp_stdin = dup(STDIN_FILENO);
	    dup2(fd, STDIN_FILENO);
	    execute(argv);
	    dup2(temp_stdin, STDIN_FILENO);
	  }
	  else {
	    wait(&status);
	  }
	} //end stdin
	//if pipe
	else if (redir[0] == '|') {
	  arg = parse(args[i],"|"); 
	  int pipes[2];
	  if (pipe(pipes) == -1) {
	    printf("Pipe failed\n");
	  }
	  else {
	    int temp_stdout = dup(STDOUT_FILENO); // first command
	    dup2(pipes[1],STDOUT_FILENO);
	    argv = parse(arg[0]," ");
	    execute(argv);
	    close(pipes[1]);
	    dup2(temp_stdout,STDOUT_FILENO);

	    int temp_stdin = dup(STDIN_FILENO); //second
	    dup2(pipes[0], STDIN_FILENO);
	    argv = parse(arg[1]," ");
	    execute(argv);
	    close(pipes[0]);
	    dup2(temp_stdin, STDIN_FILENO);
	    
	  }

	}



	free(argv);
	free(arg);
      }
      
      else {
	argv = parse(args[i]," ");

	execute(argv);
	free(argv);
      }
      i++;

    }
    free(args);
    //argv = parse(input," ");
    //execute(argv);
    //free(argv);
  }

  return 0;
}
