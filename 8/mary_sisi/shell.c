#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

void print_prompt();
void print_array(); //for testing purposes
void parse(char ** a); //parses user input
int contains(char ** a, char * c); //helper
int execute(char ** a); //hanldes user input
void allocate_array_mem(char ** a, int i);


/*

POTENTIAL IMPROVEMENTS:

- make parse recognize characters such as '<', '>', '|', ';', etc. so as to separate them into separate strings, and handle cases with spaces on one side (e.g. "ls;wc", "ls; wc", and "ls ;wc")

- factor out code for memory allocation for arrays in parse() and execute()

 */


int main(){

  print_prompt();
  //int run = 1;

  while(1){ //run){

    char ** args; //= (char**)malloc(sizeof(char *) * 64);
    parse(args);
    execute(args);
    print_prompt();

  }
  
  return 0;

}


void print_prompt(){

  char path[256];
  getcwd(path, 256);  
  printf("%s$ ", path);

}


void print_array(char ** args){

  int i = 0;

  while(args[i]){
    printf("args[%d]: %s\t", i, args[i]);
    i++;
  }

  printf("\n");

}


//args is a buffer
//call parse(args) to take user input from stdin and parse it into an array of strings which will become accessible from args
void parse(char ** args){

  //make char array, s1, to hold user input
  char s1[256];
  //put the user input in s1
  fgets(s1, sizeof(s1), stdin);
  //and get rid of the newline at the end
  s1[strlen(s1)-1] = '\0';

  //make a char pointer, s, to the beginning of s1, which will later be used to iterate through s1
  char * s = s1;
  //make a char pointer, temp, which will aid s in iterating through s1
  char * temp = strsep(&s, " ");

  //after using strsep:
  //s now points to the first argument provided by the user, which is now be followed by a terminating null
  //temp points to the remainder of the user input (starting at the second argument, if it exists, and terminating after the end of the user input)

  //make an array of char pointers, args, which will hold the parsed values as separate entities
  //char * args[64];
  //allocate memory for each element in args using a while loop
  int i = 0;
  while(i < 64){
    args[i] = (char *)malloc(64, sizeof(char));
    i++;
  }
  //MIGHT BE ABLE TO REPLACE THE ABOVE WITH ALLOCATE_ARRAY_MEM(ARGS, 64) OR WHATEVER INTEGER VALUE IF SAID FUNCTION ACTUALLY WORKS

  //reset the counter variable for use in the next loop
  i = 0;

  //until temp hits the terminating null in s1...
  while(temp){
    
    //as long as temp is pointing to a non-empty value...
    if(strcmp(temp,"") != 0){
      //copy value at temp into allocated space at args[i]
      strcpy(args[i], temp);
      i++;   
    }

    //(if there are multiple spaces between arguments, the if statement will prevent the extra spaces from being added to args)

    //move to next argument
    temp = strsep(&s," ");
  }

  //terminate args with a 0
  args[i] = 0;

}


//returns -1 if c is not found in args
//returns the index of first occurrence of c in args otherwise
int contains(char ** args, char * c){

  int i=0;

  while(args[i]){

    if (strcmp(args[i], c) == 0 ){
      return i;
    }

    i++;

  }

  return -1;

}


int execute(char ** args){
  if(contains(args) == 0){//regular input no ; < > |(
    if (strcmp(args[0], "exit") == 0){
      printf("BYE!!!!\n");
    }
    else if  (strcmp(args[0], "cd") == 0){
      chdir(args[1]);      
    }
    else{
      int f = fork();
      int status;
      if( !f ){
	execvp(args[0], args );
	//everything else
	//redirection
      }else{
	wait(&status);
      }
    }
  }else if (contains(args) == 1){ // if has semi colon
    char ** part1 =  (char**)malloc(sizeof(char *) * 64);
    int j = 0;
    while( args[j]){   
      if( strcmp(args[j], ";") != 0 ){
	part1[j] = args[j];
	j++;
      }else{
	execute(part1);
	j++;
	// this is super inefficent but it does work
	char ** part2 =  (char**)malloc(sizeof(char *) * 64);
	int i = 0;
	while (args[j]){
	  part2[i] = args[j];
	  j++;
	  i++;
	}
	execute(part2);	
      }
    }
  }
 
  return 0;  
}
