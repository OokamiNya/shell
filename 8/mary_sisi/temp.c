#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

void print_prompt();
void print_array(char** args); //for testing purposes
void parse(char ** a); //parses user input
int contains(char ** a, char * c); //helper
int execute(char ** a); //hanldes user input
void allocate_array_mem(char ** a, int i);


void print_array(char ** args){
  int i = 0;
  while(args[i]){
    printf("args[%d]:  %s\t",i,args[i]);
    i++;
  }
}

void print_prompt(){
  char path[256];
  getcwd(path, 256);  
  
  printf("%s$ ", path);
}


void parse(char ** args){

  char s1[256];
  fgets(s1, sizeof(s1), stdin);
  s1[strlen(s1)-1]='\0';

  char * s = s1;
  char * temp = strsep(&s, " ");

  int i=0;
  while(i<64){
    args[i] = (char*)calloc(64,sizeof(char));
    i++;
  }

  i=0;
  while(temp){
        if(strcmp(temp,"") != 0){
      strcpy(args[i],temp);
      i++;   
    }
    temp = strsep(&s," ");
  }

  //terminate args
  args[i] = 0;
}

void allocate_array_mem(char ** buffer, int i){
  int j = 0;
  while(j > i){
    buffer[j] = (char *)malloc(64 * sizeof(char));
    j++;
  }

}

int contains(char ** args, char * c){
  //printf("IN CONTAINS: %s\n", c);
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
  int i;
  if( (i = contains(args,";")) != -1){
    printf("COMMAND WITH ;;;;;;; %d\n", i);

    char ** part1 = (char**)malloc(sizeof(char*) * i);
    //allocate_array_mem(part1, i+1);
    
    int j = 0;
    while(j < i){
      printf("in loop\n");
      part1[j] = args[j];
      printf("broken\n");
      j++;
    }
    
    j=0;
    /*while(j<=i){
      free(args[i]);
      j++;
      }*/
    
    args += (i + 1);

    execute(part1);
    //printf("GOT TO PART 2222");
    execute(args);
    
  }

  else if( (i = contains(args,"<")) != -1  ){
    printf("COMMAND WITH <<<<<\n");
    int f = fork();
    int status;
    if (!f){
      printf("i: %d\n", i);

      int fd = open("foo.txt", O_RDWR | O_CREAT, 0644);
      //dup2(STDIN_FILENO , fd);
      printf("WORKING????");


      char * s = (char *) malloc(sizeof(char*));
      int r = read(fd,s , sizeof(s));
      close(fd);

      char ** part1 = (char**)malloc(sizeof(char*) * i);   
      //allocate_array_mem(part1, i);
    
      int j = 0;
      while(j < i){
	part1[j] = args[j];
	j++;
      }

      part1[j] = s;
      part1[j+ 1] = 0;

      print_array(part1);
     
      /* j=0;
      while(j<=i){
	free(args[i]);
	j++;
	}*/

      execvp(part1[0] , part1);
      //close, and restting stdout not nessecarry b/c its a child
    }else{
      wait(&status);
    }
  }

  else if((i = contains(args,"cd")) != -1){
    chdir(args[1]);
    //~ doesn't  work
  }else if((i = contains(args,"exit")) != -1){
    exit(-1);
  }else{
    //printf("REGULAR COMMAND\n");

    //fork and exec
    int f = fork();
    int status;
    if(!f){
      print_array(args);
      execvp(args[0], args);
    }else{
      //the parent will wait until the child has finished running
      wait(&status);
    }

  }

  return 0;  
}

int main(){

  print_prompt();
  int run = 1;
  while(run){

    char ** args = (char**)malloc(sizeof(char *) * 64);//64?
    parse(args);

    execute(args);
    print_prompt();
  }

  return 0;
}


