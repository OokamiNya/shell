#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

void print_prompt();
void parse(char ** a); //parses user input into an array of strings
int contains(char **a);
int execute(char **a); //runs exec

int main(){

  printf("\nSisi -- try typing a command with multiple arguments and as many spaces as you want, but not ; or < or > or | -- parse returns correctly now\n");
  printf("\nUnfortunately, this does nothing else yet; but the returning works, so that's great!\n");
  printf("\n... for now.\n\n");

  print_prompt();

  while(1){

    char * a[64];
    parse(a);

    int i=0;
    while(a[i]){
      printf("a[%d]: %s\n",i,a[i]);
      i++;
    }
  
    /*
    if(contains(a,";")){
      //run
    }else if(contains(a)){

    }else{
    
    }
    */
  }

  return 0;

}


void print_prompt(){
  char path[256];
  getcwd(path, 256);   
  printf("%s$ ", path);
}


void parse(char ** args){

  //make char array, s1, to hold user input
  char s1[256];
  //put the user input in s1
  fgets(s1, sizeof(s1), stdin);
  //and get rid of the newline at the end
  s1[strlen(s1)-1]='\0';

  //make a char pointer, s, to the beginning of s1, which will later be used to iterate through s1
  char * s = s1;
  //make a char pointer, temp, which will aid s in iterating through s1
  char * temp = strsep(&s, " ");

  //after using strsep:
  //s now points to the first argument provided by the user, which is now be followed by a terminating null
  //temp points to the remainder of the user input (starting at the second argument, if it exists, and terminating after the end of the user input)

  //make an array of char pointers, args, which will hold the parsed values as separate entities
  //char * args[64];
  //malloc space for each element in args using a while loop
  int i=0;
  while(i<64){
    args[i] = (char*)calloc(64,sizeof(char));
    i++;
  }

  //resetting the counter variable
  i=0;

  //until temp hits the terminating null in s1...
  while(temp){
    
    //as long as temp is pointing to a non-empty value...
    if(strcmp(temp,"") != 0){
      //copy value at temp into allocated space at args[i]
      strcpy(args[i],temp);
      i++;   
    }

    //(if there are multiple spaces between arguments, the if statement will prevent the extra spaces from being added to args)

    //move to next argument
    temp = strsep(&s," ");
  }

  //terminate args
  args[i] = 0;

  //return args;

}


//not yet implemented (or considered, basically) - multiple redirections in the same line (probably requires recursion, but we'll handle it later)
int execute(char ** args){

  //i is to be used as a location marker in args
  int i;

  if(i = contains(args,";")){
    //part1 will contain the first line of command to be run
    char ** part1;
    //allocate space for its contents
    allocate_array_mem(part1, i);
    //copy over the aforementioned first line of command from args into part1
    while(i > 0){
      part1[i]=args[i];
    }

    //execute first set of commands (*also recursive)
    //recurse and execute next, etc.
  }

  if(i = contains(args,"<")){
    //redirects [command(s) in a] file to stdin
  }else if(i = contains(args,">")){
    //redirects stdout to a file
  }else if(i = contains(args,"|")){
    //output of 1st to input of 2nd
  }else if(i = contains(args,"cd")){
    //covered
  }else if(i = contains(args,"exit")){
    //covered
  }else{
    //fork and execvp
  }

}


//allocates memory for a string of 64 characters for each element in the array, the number of elements being indicated by i
void allocate_array_mem(char ** buffer, int i){

  while(i > 0){
    buffer[i] = (char *)malloc(64, sizeof(char));
    i--;
  }

}
