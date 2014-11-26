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
            if ( split_cmds[0] == ' ' ) {
                split_cmds = &split_cmds[1];
            }
            if ( split_cmds[ strlen( split_cmds ) - 1 ] == ' ') {
                split_cmds[ strlen( split_cmds ) - 1 ] = 0;
            }
            
            if ( strchr( split_cmds, '>' ) != 0 ) {
                char * cmd;
                cmd = strsep( &split_cmds, ">" ); //gets cmd from input
                temp_stdout = dup( STDOUT_FILENO ); //creates copy of STDOUT
                fd = open( split_cmds, O_WRONLY|O_CREAT, 0644 ); //opens file from input
                dup2( fd, STDOUT_FILENO ); //redirects STDOUT to file
                execute( cmd ); //runs cmd
                dup2( temp_stdout, STDOUT_FILENO ); //resets STDOUT to normal, not file
                close( fd );
            }
            else if ( strchr( split_cmds, '<' ) != 0 ) {
                char * cmd;
                cmd = strsep( &split_cmds, "<" ); //gets cmd from input
                temp_stdin = dup( STDIN_FILENO ); //creates copy of STDIN
                fd = open( split_cmds, O_WRONLY|O_CREAT, 0644 ); //opens file from input
                dup2( fd, STDIN_FILENO ); //redirects STDIN to file
                execute( cmd ); //runs cmd
                dup2( temp_stdin, STDIN_FILENO ); //resets STDIN to normal, not file
                close( fd );
            }
            else if ( strchr( split_cmds, '|' ) != 0 ) {
                char * cmd1;
                char output1[500];
                cmd1 = strsep( &split_cmds, "|" ); //gets 1st cmd from input
                temp_stdout = dup( STDOUT_FILENO ); //creates copy of STDOUT
                //fd = open( split_cmds, O_WRONLY|O_CREAT, 0644 ); //opens file from input
                //dup2( STDOUT_FILENO, STDIN_FILENO ); //redirects STDIN to STDOUT
                execute( cmd1 ); //runs cmd1
                //fgets( output1, sizeof( output1 ), stdin ); //maybe something with this
                dup2( temp_stdout, STDOUT_FILENO ); //resets STDOUT to normal, not STDIN
                execute( split_cmds ); //runs cmd2
            }
            else {
                execute( split_cmds );
            }
        }
    }
    return 0;
}