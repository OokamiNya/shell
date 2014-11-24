#include "shell.h"

int i = 1;

void execute( char * args[] ) {
  int f;
    
  if ( !strcmp(args[0], "cd") ) {
    chdir( args[1] );
  }
  else if ( !strcmp(args[0], "exit") ) {
    i = 0;
  }
  else{
    f = fork();
    if ( !f ) {
      execvp(args[0], args);
    }
    else {
      wait(&f);
    }
  }
}

int main(){
  printf( "Welcome to the The Magic Conch Shell.\n" );
    
    
  char input_str[100];
  char cwd[200];
  while( i ) {
    getcwd( cwd, sizeof(cwd) );
    printf( "%s$ ", cwd );
        
<<<<<<< Updated upstream
    fgets( input_str, sizeof(input_str), stdin );
    char *temp = input_str; //strsep needs char*, not char[]
    char *raw_str = strsep(&temp, "\n"); //removes ending line break
        

    char *split_cmds; //holds the piece separated off from raw_str (a cmd + arg string)

    int c = 0;
    char *split_args; //holds the piece separated off from x
    char * args[10]; //arg array
    char * redirect;
    //if (restrchr(raw_str,">") != -1){
      
    while( (split_cmds = strsep(&raw_str, ";")) ) {
      c++;
      int a = 0;
      //printf("cmd: %s\n",split_cmds);
      if (split_cmds[0] == ' '){
	split_cmds= &split_cmds[1];
      }
      while( (split_args = strsep(&split_cmds, " ")) ) {
	args[a] = split_args;
	a++;
      }
      args[a]=0;
      execute(args);
=======
        int c = 0;
        char *split_args; //holds the piece separated off from x
        char * args[10]; //arg array
        char * redirect;
        int fd=0;
        //int temp_stdin;
        //int temp_stdout;
        while( (split_cmds = strsep(&raw_str, ";")) ) {
            c++;
            int a = 0;
            //printf("cmd: %s\n",split_cmds);
            if (split_cmds[0] == ' '){
                split_cmds= &split_cmds[1];
            }
            if (split_cmds[strlen(split_cmds)-1] == ' '){
                split_cmds[strlen(split_cmds)-1]=0;
                //printf("space at end: %s\n",split_cmds);
            }
            
            /*if (&(redirect=strchr(split_cmds,'>')) != 0){
                redirect=0;
                fd=open(redirect+1);
                temp_stdout = dup(STDOUT_FILENO);
                dup2(fd, stdout);
            }
            if (&(redirect=strchr(split_cmds,'<')) != 0){
                redirect=0;
                fd=open(redirect+1);
                temp_stdin = dup(STDIN_FILENO);
                dup2(fd,stdout);
            }*/
            
            while( (split_args = strsep(&split_cmds, " ")) ) {
                args[a] = split_args;
                a++;
            }
            args[a]=0;
            execute(args);
            /*if (fd){
                dup2(temp_stdin,fd);
                dup2(temp_stdout,fd);
            }*/
            
            
            
        }
>>>>>>> Stashed changes
    }
  }
  return 0;
}


