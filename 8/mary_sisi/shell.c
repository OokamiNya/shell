#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

//void print_prompt();
//char ** parse();
//int contains(char **args, char keys);
//int handle(char **args);

/*
still don't get why that whole returning thing isn't working. because now i put execute
 seperately in its own function to make main less clutteres and args will pass through 
to another function but won't return from the parse function. I think you were on to somehting
in class when you tried to change the return type of the parse function. But anyway, for not I
tried mocing on from that extraordinarily annoying isssue

 */


/*Ok, so the parse is sort of working. If you test it out, you can see that
at the end of the pasre function i print it out and each element is correct
but then when i try to print it in main it doesn't work
So when the value is being returned is where it gets messed up somehow
 
*/

//parse
char** parse(){

    char s1[256];
    fgets(s1, sizeof(s1), stdin);
    s1[strlen(s1)-1]='\0';

    char * s = s1;
    char ** args = (char**)malloc(sizeof(char *) * 64);//64?
    char * temp = strsep(&s, " ");

    int i=0;
    while(temp){
      args[i] = temp;    
      temp = strsep(&s, " ");
      i ++;   
    }
    args[i] = 0;
    
    /* testing that parsing works
      i=0;
      while(args[i]){
      printf("args[%d]:  %s\t",i,args[i]);
      i++;
      }*/
    
    //printf("inside parse: %d\n",&args);

    return args;

}

int contains(char** args){

  //xprintf("IN CONTAINS\n");

  //  testing that parsing works
  int i=0;
  while( args[i] ){
    if ( strcmp(args[i], ";") == 0 ){
      return 1;
    }//else if:  > < | return 2 3 4


    i++;
  }
  return 0;
}


int redirection (char * source, char * dest){

  return 0;
}

void print_prompt(){
  char path[256];
  getcwd(path, 256);  
  
  printf("%s$ ", path);
}

int execute(char ** args){
  //RUNNING PARSED CODE
  if (strcmp(args[0], "exit") == 0){
    //run = 0;
    printf("BYE!!!!\n");
  }
  else if  (strcmp(args[0], "cd") == 0){
    chdir(args[1]);
    print_prompt();
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
      print_prompt();
    }
  }
  return 0;
}

int main(){

  print_prompt();
  int run = 1;
  while(run){
    
    //PARSING
    char s1[256];
    fgets(s1, sizeof(s1), stdin);
    s1[strlen(s1)-1]='\0';
    
    char * s = s1;
    char ** args = (char**)malloc(sizeof(char *) * 64);//64?
    char * temp = strsep(&s, " ");

    int i=0;
    while(temp){
      if(strcmp(temp,"") != 0){ // deals with aditional spaces issue
	args[i] = temp;  
	i ++;   
      } 
      temp = strsep(&s, " ");
    }
    args[i] = 0;
    
    /* i=0;
    while(args[i]){
      if(
      printf("args[%d]:  %s\t",i,args[i]);
      i++;
      }*/

    //execute(args);

    if(contains(args) == 0){//regular input no ; < > |
      execute(args);
    }

    //not exactly sure how i'm trying to do this, but this is the general idea of the ; part
    /*else if(contains(args) == 1){//has ;
      printf("has semicolon!!\n");
      char ** part;
      int j = 0; // counter for index of args
      int k = 0;//counter for index of part
      while( args[j]){
	if( strcmp(args[j], ";") != 0 ){
	  part[k] = args[j];
	  k++;
	}else{
	  part[k] = 0;
	  execute(part);
	  int k = 0;
	}
	j++;
      }
      

      }*/
    
  }


  
  return 0;
}


