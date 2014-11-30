#include "shell.h"

//Input: char* input
//Returns: an array of strings with the command and the args
//>Main parsing function of this terminal
//>Cuts up the input and puts them into an array
//>Will be cut up based on where there are spaces
//>Will get rid of the newline at the end, from fgets
//>First string in the array is the command, the rest are args
char** parse_command(char* input) {
  char** output;
  char* new_input;
  char* temp;
  output = (char**)malloc(16*sizeof(char));  
  new_input = (char*)malloc(256*sizeof(char));
  temp = (char*)malloc(256*sizeof(char));

  int i = 0;
  input = remove_newline(input);
  strcpy(new_input, input);

  if (strstr(new_input, " ")) {
    while(temp = strsep(&new_input, " ")) {
      output[i] = (char*)malloc(256*sizeof(char));
      strcpy(output[i], temp);
      i++;
    }
  } else {
    output[0] = (char*)malloc(256*sizeof(char));
    strcpy(output[0], new_input);
  }
  i = 0;
  
  return output;
}

//Input: char** args
//Returns: status of the child
//>Will fork and run the command given to it by parse_command()
//>Simple function that just runs a single command
int exec_command(char** args) {
  int f, status;
  f = fork();

  if (f==0)
    execvp(args[0], args);
  else 
    wait(&status);

  return status;
}

//Input: char* input
//Returns: 0
//>Wrapper function to check whether the command contains ;, cd, >, or <
//>Will run parse_command() and exec_command() if it is another command
int check_and_run(char* input) {
  char** cmd;
  
  if(!check_command(input)) {
    cmd = parse_command(input);
    exec_command(cmd);
  }
  return 0;
}

//Input: char* source, char* dest
//Returns: 0
//>Handles the redirection that is given by > and <
//>Will redirect STDOUT and then run the command, given by source
int redirect(char* source, char* dest) {
  int fd_source, fd_dest;
  char temp[256] = "touch ";
  strcat(temp, dest);
  check_and_run(temp);
  fd_dest = open(dest, O_WRONLY | O_TRUNC);
  fd_source = dup(STDOUT_FILENO);
  dup2(fd_dest, STDOUT_FILENO);
  check_and_run(source);
  dup2(fd_source, STDOUT_FILENO);

  return 0;
}

//Input: char* input
//Returns: input without the \n at the end, replaced by a \0
//>Runs through input and replaces all \n occurences with \0
char* remove_newline(char* input) {
  int i = 0;

  while(input[i]) {
    if(!strcmp(&input[i], "\n"))
      input[i] = 0;
    i++;
  }

  return input;
}

//Input: char* input
//Returns: 1 if ;, cd, >, or < was run, 0 if nothing was run
//>Checks if ;, cd, >, or < was in the command and runs the necessary
//functions if they were in the code
//>Main function that checks for special types of commands
int check_command(char* input) {
  int ret_val = 0;
  char** cmd;
  char* temp;
  
  if (!strcmp(input, "\n")) {
    ret_val++;
  } else if (strstr(input, ";")) {
    temp = strsep(&input, ";");
    check_and_run(temp);
    check_and_run(input);
    ret_val++;
  } else if(strstr(input, "cd")) {
    cmd = parse_command(input);
    chdir(cmd[1]);
    opendir(cmd[1]);
    ret_val++;
  } else if(strstr(input, ">")) {
    temp = strsep(&input, ">");
    input = remove_newline(input);
    redirect(temp, input);
    ret_val++;
  } else if(strstr(input, "<")) {
    temp = strsep(&input, "<");
    input = remove_newline(input);
    redirect(input, temp);
    ret_val++;
  } 
  /*
    else if(strstr(input, "|")) {
    temp = strsep(&input, "|");
    input = remove_newline(input);
    piping(temp, input);
    ret_val++;
    } 
    else if(strstr(input, ">>")) {
    printf("Found it");
    temp = strsep(&input, ">>");
    input = remove_newline(input);
    redirect_append(temp, input);
    ret_val++;
    } else if(strstr(input, "<<")) {
    temp = strsep(&input, "<<");
    input = remove_newline(input);
    redirect_append(input, temp);
    ret_val++;
    }
  */

  return ret_val;
}

/*
  int redirect_append(char* source, char* dest) {
  int fd_source, fd_dest;
  //char temp[256] = "touch ";
  //strcat(temp, dest);
  //check_and_run(temp);
  fd_dest = open(dest, O_WRONLY | O_APPEND);
  fd_source = dup(STDOUT_FILENO);
  dup2(fd_dest, STDOUT_FILENO);
  check_and_run(source);
  dup2(fd_source, STDOUT_FILENO);

  return 0;
  }
*/

/*
  int piping(char* source, char* dest) {
  
  char string[SIZE];
  freopen("/dev/null", "a", stdout);
  setbuf(stdout, string);

  return 0;
  }
*/
