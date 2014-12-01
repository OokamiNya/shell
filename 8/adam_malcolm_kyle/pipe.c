#include "pipe.h"

extern int f;
void piping(char * args[], int max){
  //piping: Takes a char pointer array with commands that were split by "|"                                              
  //Sends each command to appropriate methods                                                                            
  //Returns nothing                                                                                                      
  start(args[0]);

  int step = 1;
  while (step < max){
    mid(args[step]);
    step++;}

  end(args[max]);

  unlink("woo.txt");                                                                                                   
}
void start(char * command){
  //Takes a command representing the first command in a pipe                                                             
  //Runs it and stores it's output in a file                                                                             
  char * args[10];
  int x = 0;
  char * y;
  int redir = 0;
  while (command){
    y = strsep(&command, " ");
    if(*y != 0){
      args[x] = y;
      x++;
    }
  }
  args[x] = 0;
  int c;
  f = fork();
  if (!f){
    c = open("woo.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);
    dup2(c,STDOUT_FILENO);
    execvp(args[0],args);
    exit(0);
  }
  else{
    wait(&f);
    f = 0;
    close(c);
  }
}
void mid(char * command){
  //Takes a command representing any middle arguement of a pipe                                                          
  //Runs it using info stores in a file, and stores it's output in same file                                             
  char * args[10];
  int x = 0;
  char * y;
  int redir = 0;
  while (command){
    y = strsep(&command, " ");
    if(*y != 0){
      args[x] = y;
      x++;
    }
  }
  args[x] = 0;
  int c;
  f = fork();
  if (!f){
    c = open("woo.txt", O_RDWR | O_TRUNC, 0777);
    dup2(c,STDOUT_FILENO);
    dup2(c,STDIN_FILENO);
    execvp(args[0],args);
    exit(0);
  }
  else{
    wait(&f);
    f = 0;
  }
}
void end(char * command){
  //Takes a command representing any end arguement of a pipe                                                             
  //Runs it using info stores in a file, and prints out it's output                                                      
  char * args[10];
  int x = 0;
  char * y;
  int redir = 0;
  while (command){
    y = strsep(&command, " ");
    if(*y != 0){
      args[x] = y;
      x++;
    }
  }
  args[x -1] = strsep(&args[x-1], "\n");
  args[x] = 0;
  int d = 0;
  int c;
  f = fork();
  if (!f){
    c = open("woo.txt",O_RDONLY);
    dup2(c,STDIN_FILENO);
    execvp(args[0],args);
    exit(0);
  }
  else{
    wait(&f);
    f = 0;
  }
}
