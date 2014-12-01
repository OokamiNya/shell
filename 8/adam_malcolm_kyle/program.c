#include "program.h"
#include "pipe.h"
int f =0;

char home[256] = "/";

void process(char * start){
  //site of user input processing
  char * y;
  while (start){
    y = strsep(&start, ";");
    if (*y != 0)
      execute(y);
  }
  
}


void execute(char * start){
  //execute: Takes a char pointer start representing the beginning f a String
  //Splits the string arguements and sends to appropriate method
  //Returns nothing
  char * test[10];
  int x =0;
  char * temp;
  while (start){
    temp = strsep(&start, "|");
    if(*temp != 0){
      test[x] = temp;
      x++;
    }
  }
  if (x > 1){
    test[x] = 0;
    piping(test,x - 1);}
  else{
    start = temp;
    if( ! strncmp(start,"HOME=",5))
      set_home(start);
    else{
      char * args[10];
      x = 0;
      char * y;
      int redir = 0;
      while (start){
	y = strsep(&start, " ");
	if(*y != 0){
	  if (! strcmp(">>",y))
	    redir = 3;
	  else if( ! strcmp("<",y))
	    redir = 2;
	  else if (! strcmp(">",y))
	    redir = 1;
	  args[x] = y;
	  x++;
	}
    
      }
      args[x -1] = strsep(&args[x-1], "\n");
      args[x]=0;
      if (redir){
	args[x + 1] = 0;
	redirect(args, redir);
      }
      else{
	if (! (strcmp("cd",args[0]) && strcmp("exit",args[0])))
	  normal_process(args);
	else{
	  child_process(args);
	}
      }
    }
  }
}
void set_home(char * start){
  //Sets the 'home' directory which allows for the use of the character "~"
  //Takes the location of new home directory in relation to current directory
  getcwd(home,256);
  strsep(&start, "=");
  start = strsep(&start, "\n");
  strcat(home,"/");
  strcat(home,start);
}
void normal_process(char * args[]){
  //For processes that require the main process to run
  //Takes the process with args as an array
  if (! (strcmp("cd",args[0]))){
    if (! strcmp(args[1],"~")){
      chdir(home);
    }
    chdir(args[1]);
  }
  else if (! (strcmp("exit",args[0])))
    exit(0);
}

void child_process(char * args[]){
  //For processes that require a child to run
  //Takes the process with args as an array
  f = fork();
  if (!f){

    if(!strcmp("bug",args[0])){
      printf("           _         _\n          /x\\       /x\\\n         /v\\x\\     /v\\/\\\n         \\><\\x\\   /></x/\n          \\><\\x\\ /></x/\n  __ __  __\\><\\x/></x/___\n /##_##\\/       \\</x/    \\__________\n|###|###|  \\         \\    __________\\\n \\##|##/ \\__\\____\\____\\__/          \\\\\n   |_|   |  |  | |  | |              \\|\n   \\*/   \\  |  | |  | /              /\n           /    /\n");

    }
    if(strcmp("flood_my_terminal",args[0])){
      execvp(args[0],args);
      exit(0);}
    else{
      while(1){
	sleep(1);
	printf("Would you really kill me?\n");
      }
    } 	 
  }
  else{
    wait(&f);
    f = 0;
    if(! strcmp("flood_my_terminal",args[0]))
      printf("\nI see you have no mercy\n");
    
  }
}


void redirect(char * args[], int redir){
  //redirect: Takes a char pointer array with command line input and int representing type of redirection
  //Performs appropriate redirection depending on ">", "<", or ">>"
  //Allows for <command> > <file1> > <file2> etc. if that's what you'd like
  //Returns nothing
  int c;
  f = fork();
  if(!f){
    if (redir == 3){
      int x =2;
      while (args[x]){
	int d = fork();
	if (!d){
	  c = open(args[x], O_CREAT | O_WRONLY| O_APPEND, 0644);
	  dup2(c, STDOUT_FILENO);
	  close(c);
	  execlp(args[0], args[0], NULL);
	  exit(0);
	}
	else{
	  wait(&d);
	  x+=2;
	}
      }
    }
    else if (redir == 2){
      c = open(args[2],O_RDWR,0777);
      dup2(c,STDIN_FILENO);
      close(c);
      execlp(args[0],args[0],NULL);
      exit(0);
    }else{
      int x =2;
      while (args[x]){
	int d = fork();
	if (!d){
	  c = open(args[x], O_CREAT | O_WRONLY, 0644);
	  dup2(c, STDOUT_FILENO);
	  close(c);
	  execlp(args[0], args[0], NULL);
	  exit(0);
	}
	else{
	  wait(&d);
	  x+=2;
	}
      }
    }
  }else{
    wait(&f);
    f = 0;
    redir = 0;
  }
}

static void sighandler( int signo ) {
  //Takes the signal number
  //Only kills a process if it has no children
    if ( signo == SIGINT ) {
        if ( !f ) {
            exit(0);
        }
    }
}

int main(int argc, char *argv[]){
  signal( SIGINT, sighandler );
  char directory[256];
  while(1){
    getcwd(directory,256);
    char * direct_point = &directory[0];
    char * args[15];
    int x = 0;
    while (direct_point){
      args[x] = strsep(&direct_point, "/"); 
      x++;
    }
    printf("%s/%s: ",args[x-2],args[x-1]);
    char input[256];
    fgets(input, 256, stdin);
    printf("\n");
    char * start = &input[0];
    process(start);  
    
  }
  return 0;
}
    
