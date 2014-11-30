/*

have to fix:
- freeing (in main)

simple optional features:
- sighandler for Ctrl+C (attempted, doesn't work yet)
- implement >> and <<
- make parse() work without spaces between everything

not-so-simple optional features:
- up arrow to view history
- tab to display and/or fill in options while typing
- * as a wildcard
- & to run things in the background
- ~ as a directory shortcut
- assign values to variables and such

non-coding related things to do:
- write readme.txt about project, when we're done
- makefile
- header file?

 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

static void sighandler(int signo);
void print_prompt();
void print_array(char** args); //for testing purposes
void parse(char ** a); //parses user input
int contains(char ** a, char * c); //helper
int execute(char ** a); //hanldes user input


int main(){

  signal(SIGINT, sighandler);

  print_prompt();
  int run = 1;
  while(run){

    char ** args; //allocate space for up to 64 strings of up to 32 characters each
    args = (char**)malloc(sizeof(char)*64);
    char ** temp = args;

    int i = 0;
    while(i < 32){
      args[i] = (char*)malloc(sizeof(char)*32);
      i++;
    }

    parse(args);
    execute(args);
 
    //freeing doesn't work, gotta fix that
    /* i = 0; */
    /* while(i < 32){ */
    /*   free(temp[i]); */
    /*   i++; */
    /* } */
    /* free(temp); */

    print_prompt();
  }

  return 0;
}


//I wish this worked.  It does not.
static void sighandler(int signo){
  printf("\n");
  main();
}


void print_prompt(){
  char path[256];
  getcwd(path, 256);  
  
  printf("%s$ ", path);
}


//for testing purposes
void print_array(char ** args){
  int i = 0;
  while(args[i]){
    printf("args[%d]:  %s\t",i,args[i]);
    i++;
  }
  printf("\n");
}


void parse(char ** args){

  char s1[256];
  fgets(s1, sizeof(s1), stdin);
  s1[strlen(s1)-1]='\0';

  char * s = s1;
  char * temp = strsep(&s, " ");

  int i=0;
  while(temp){
    if(strcmp(temp,"") != 0){
      strcpy(args[i],temp);
      i++;   
    }
    temp = strsep(&s," ");
  }

  //termination
  args[i] = NULL;
}


int contains(char ** args, char * c){
  int i=0;
  while(args[i]){
    if (strcmp(args[i], c) == 0){
      return i;
    }
    i++;
  }
  return -1;
}


int execute(char ** args){

  int i;

  if((i = contains(args,";")) != -1){
    //printf("\nCOMMAND WITH ';' AT INDEX %d\n\n", i);

    char ** part1 = (char**)malloc(sizeof(char*) * i);
    
    int j = 0;
    while(j < i){
      part1[j] = args[j];
      j++;
    }
    
    args += (i + 1);

    execute(part1);
    execute(args);
    
  }else if((i = contains(args,"<")) != -1  ){
    //printf("\nCOMMAND WITH '<' AT INDEX %d\n\n",i);
    
    int f = fork();
    int status;

    if (!f){
      int fd = open(args[i+1], O_RDWR | O_CREAT, 0644);
      dup2(fd, STDIN_FILENO);

      char ** part1 = (char**)malloc(sizeof(char*) * i);
      
      int j = 0;
      while(j < i){
	part1[j] = args[j];
	j++;
      }

      execvp(part1[0], part1);
      //in case execvp doesn't run:
      if(1){
	kill(getpid(),SIGTERM);
      }
      //it isn't necessary to free part1 or reset the file table values, since the child is killed

    }else{
      wait(&status);
    }

  }else if((i = contains(args,">")) != -1  ){
    //printf("\nCOMMAND WITH '>' AT INDEX %d\n\n",i);
    
    int f = fork();
    int status;

    if (!f){
      int fd = open(args[i+1], O_RDWR | O_CREAT, 0644);
      dup2(fd, STDOUT_FILENO);

      char ** part1 = (char**)malloc(sizeof(char*) * i);
      
      int j = 0;
      while(j < i){
	part1[j] = args[j];
	j++;
      }

      execvp(part1[0], part1);
      //in case execvp doesn't run:
      if(1){
	kill(getpid(),SIGTERM);
      }
      //it isn't necessary to free part1 or reset the file table values, since the child is killed

    }else{
      wait(&status);
    }

  }else if((i = contains(args,"|")) != -1  ){
    //printf("COMMAND WITH '|' AT INDEX %d\n",i);

    char ** part1 = (char**)malloc(sizeof(char*) * (i + 3));
    int j = 0;
    while(j < i){
      part1[j] = args[j];
      j++;
    }
    part1[j] = ">";
    part1[j+1] = "buffer.txt";
    part1[j+2] = NULL;

    /* printf("part1: "); */
    /* print_array(part1); */

    args += (i + 1);

    //print_array(args);

    if((i = contains(args,";")) != -1){
    }else if((i = contains(args, "<")) != -1){
    }else if((i = contains(args, ">")) != -1){
    }else if((i = contains(args, "|")) != -1){
    }else{
      i = 0;
      while(args[i]){
	i++;
      }
      //printf("i: %d\n",i);
    }

    char ** part2 = (char**)malloc(sizeof(char*) * (i + 3));

    j = 0;
    while(j < i){
      part2[j] = args[j];
      j++;
    }

    part2[j] = "<";
    part2[j+1] = "buffer.txt";
    part2[j+2] = NULL;

    /* printf("part2: "); */
    /* print_array(part2); */

    execute(part1);
    execute(part2);

    free(part1);
    free(part2);

    remove("buffer.txt");

  }else if((i = contains(args,"cd")) != -1){
    if(!args[1]){
      chdir(getenv("HOME"));
    }else{
      /*int j = 0;
	char * path = args[1]; 
	while( j < strlen(path) ){
	if ( strcmp(path[j], "~") == 0 ){
	char * newpath;
	newpath = strcat( strsep(path),getenv("HOME") );
	  
	chdir(path2);
	}
	j ++;
	}*/
      // ^^ was trying to work on ~
      chdir(args[1]);
    }
  }else if((i = contains(args,"exit")) != -1){
    exit(-1);
  }else{
    int f = fork();
    int status;
    if(!f){
      //print_array(args);
      execvp(args[0], args);
      //in case execvp doesn't run:
      if(1){
	kill(getpid(),SIGTERM);
      }
    }else{
      wait(&status);
    }
  }

  return 0;

}
