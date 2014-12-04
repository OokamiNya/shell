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

char* rmspaces( char *str){
  if ( str[0] == ' ' ) {
    str = &str[1];
  }
  if ( str[ strlen( str ) - 1 ] == ' ') {
    str[ strlen( str ) - 1 ] = 0;
  }
  return str;
}


void calculator(char * str){
  int total=0;
  int i=0;
  str=&str[1];
  while( str[i] ){
    if (str[i] == '+'){
      str[i]=0;	
      total+=atoi(str);
      str= &str[i+1];
      i=-1;
    }
    else if (str[i] == '-'){
      str[i]=0;
      total+=atoi(str);  
      if (str[i+1] == ' '){
	str[i+1]= '-';
	str=&str[i+1];
      }
      else{
	str[i]='-';
	str= &str[i];
      }
      i=0;
    }
    i++;
  }
  total+=atoi(str);
  printf("%d\n",total);  
}

void printUnicorn(){
  printf(    "\\.\n     \\'.      ;.\n      \\ '. ,--''-.~-~-'-,\n       \\,-' ,-.   '.~-~-~~,\n     ,-'   (###)    \\-~'~=-.\n _,-'       '-'      \\=~-''~~',\n\\o                    \\~-''''~=-,\n\\__                    \\=-,~''-~,\n   ''''''===-----.         \\~=-''~-.\n               \\         \\*=~-''\n          rs    \\         ''=====----\n                 \\\n                  \\\n\nUNICORNS OF THE WORLD UNITE\n");
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
  else if ( !strcmp(args[0], "unicorn") ) {
    printUnicorn();
  }
  else {
    f = fork();
    if ( !f ) {
      execvp(args[0], args);
    }
    else {
      wait( &f );
      f = 0;
    }
  }
}

int main() {
  printf( "Welcome to the The Magic Conch Shell.\n" );
  signal( SIGINT, sighandler );
    
  char input_str[100];
  char cwd[200];
  while( i ) {
        
    getcwd( cwd, sizeof(cwd) );
    char * shortPath;
    char * savedPath;
    char * tempcwd=cwd;
    while((shortPath = strsep(&tempcwd, "/"))){
      savedPath=shortPath;
    }
    printf( "MagicConchSHELL: %s$ ", savedPath );
        
        
    fgets( input_str, sizeof( input_str ), stdin );
    char *temp = input_str; //strsep needs char*, not char[]
    char *raw_str = strsep( &temp, "\n" ); //removes ending line break
        
    char *split_cmds; //holds the piece separated off from raw_str (a cmd + arg string)
        
    char * cmd1;
    int fd = 0;
    int temp_stdin;
    int temp_stdout;
    
    while (( split_cmds = strsep(&raw_str, ";") )) {
      int a = 0;
      split_cmds=rmspaces(split_cmds);
      if ( strchr( split_cmds, '>') ) {
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
	fd = open( split_cmds, O_RDONLY, 0644 ); //opens file from input
	dup2( fd, STDIN_FILENO ); //redirects STDIN to file
	execute( cmd ); //runs cmd
	dup2( temp_stdin, STDIN_FILENO ); //resets STDIN to normal, not file
	close( fd );
      }
      else if ( strchr( split_cmds, '|' ) ) {
	char * cmd1;
	cmd1 = strsep( &split_cmds, "|" ); //gets 1st cmd from input
	rmspaces(cmd1);
	rmspaces(split_cmds);
                
	temp_stdout = dup( STDOUT_FILENO ); //creates copy of STDOUT
	temp_stdin = dup( STDIN_FILENO ); //creates copy of STDOUT
                
	int temp_f=open( ".pipe", O_WRONLY|O_CREAT, 0644 );
	dup2( temp_f, STDOUT_FILENO); //redirects STDOUT to STDIN
        
	execute( cmd1 ); //runs cmd1
	dup2( temp_stdout, STDOUT_FILENO ); //resets STDOUT to normal, not STDIN
	close(temp_f);
                
	temp_f=open( ".pipe", O_RDONLY, 0644 );
	dup2( temp_f, STDIN_FILENO);

	execute( split_cmds ); //runs cmd2
               
	dup2( temp_stdin, STDIN_FILENO );
	close(temp_f);
	int f;
	f = fork();
	if ( !f ) {
	  execlp("rm","rm",".pipe",NULL);
	}
	else {
	  wait( &f );
	  f = 0;
	}
               
      }
      else if(split_cmds[0] == '$' && (strchr( split_cmds, '+') || strchr( split_cmds, '-'))){
	calculator(split_cmds);

      }
      else {
	execute( split_cmds );
      }
    }
  }
  return 0;
}
