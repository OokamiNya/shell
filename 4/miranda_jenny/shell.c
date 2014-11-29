#include "shell.h"

int i = 1;
int f = 0;

static void sighandler( int signo ) {
  if ( signo == SIGINT ) {
    if ( !f ) {
      exit(0);
    }
  }
}

void execute( char* split_cmds ) {
  int a = 0;
  char *split_args; //holds the piece separated off from x
  char * args[10]; //arg array
  while (( split_args = strsep(&split_cmds, " ") )) {
    args[a] = split_args;
    a++;
  }
  args[a] = 0;
  if ( !strcmp(args[0], "cd") ) {
    chdir( args[1] );
  }
  else if ( !strcmp(args[0], "exit") ) {
    i = 0;
  }
  else {
    f = fork();
    if ( !f ) {
      execvp(args[0], args);
      exit(0);
    }
    else {
      wait( &f );
      f = 0;
    }
  }
}
char* rmspaces( char *str){
  if ( str[0] == ' ' ) {
    str = &str[1];
  }
  if ( str[ strlen( str ) - 1 ] == ' ') {
    str[ strlen(str ) - 1 ] = 0;
  }
  return str;
}

int main() {
  printf( "Welcome to the The Magic Conch Shell.\n" );
  signal( SIGINT, sighandler );
    
  char input_str[100];
  char cwd[200];
  while( i ) {
        
    getcwd( cwd, sizeof(cwd) );
    printf( "%s$ ", cwd );
        
        
    fgets( input_str, sizeof( input_str ), stdin );
    char *temp = input_str; //strsep needs char*, not char[]
    char *raw_str = strsep( &temp, "\n" ); //removes ending line break
        
    char *split_cmds; //holds the piece separated off from raw_str (a cmd + arg string)
        
    int c = 0;
        
    char * redirect;
    int fd = 0;
    int temp_stdin;
    int temp_stdout;
    while (( split_cmds = strsep(&raw_str, ";") )) {
      c++;
      int a = 0;
      split_cmds=rmspaces(split_cmds);
            
      if ( strchr( split_cmds, '>' ) ) {
	char * cmd;
	cmd = strsep( &split_cmds, ">" ); //gets cmd from input
	cmd=rmspaces(cmd);
	split_cmds=rmspaces(split_cmds);
	temp_stdout = dup( STDOUT_FILENO ); //creates copy of STDOUT
	fd = open( split_cmds, O_WRONLY|O_CREAT, 0644 ); //opens file from input
	dup2( fd, STDOUT_FILENO ); //redirects STDOUT to file
	execute( cmd ); //runs cmd
	dup2( temp_stdout, STDOUT_FILENO ); //resets STDOUT to normal, not file
	close( fd );
      }
      else if ( strchr( split_cmds, '<' ) ) {
	char * cmd;
	cmd = strsep( &split_cmds, "<" ); //gets cmd from input
	cmd=rmspaces(cmd);
	split_cmds=rmspaces(split_cmds);
	temp_stdin = dup( STDIN_FILENO ); //creates copy of STDIN
	printf("Taking input: %s\n",cmd);
	printf("from: %s\n",split_cmds);
	fd = open( split_cmds, O_RDONLY, 0644 ); //opens file from input
	dup2( fd, STDIN_FILENO ); //redirects STDIN to file
	execute( cmd ); //runs cmd
	dup2( temp_stdin, STDIN_FILENO ); //resets STDIN to normal, not file
	close( fd );
      }
      else if ( strchr( split_cmds, '|' ) ) {
	char * cmd1;
	char output1[500];
	char* cmd2;
	cmd2 = malloc(500 * sizeof(char));
	cmd1 = strsep( &split_cmds, "|" ); //gets 1st cmd from input
	rmspaces(cmd1);
	rmspaces(split_cmds);
	temp_stdout = dup( STDOUT_FILENO ); //creates copy of STDOUT
	temp_stdin = dup( STDIN_FILENO ); //creates copy of STDOUT
	dup2( STDOUT_FILENO, STDIN_FILENO ); //redirects STDOUT to STDIN
	printf("redirected stdout to stdin\n");
	execute( cmd1 ); //runs cmd1
	printf("about to execute cmd2\n");
	//fgets( output1, sizeof( output1 ), stdin ); //maybe something with this
	dup2( temp_stdout, STDOUT_FILENO ); //resets STDOUT to normal, not STDIN
	cmd2=strcpy(cmd2, split_cmds);
	//cmd2=strcat(cmd2,output1);
	execute( cmd2 ); //runs cmd2
      }
      else {
	execute( split_cmds );
      }
    }
  }
  return 0;
}
