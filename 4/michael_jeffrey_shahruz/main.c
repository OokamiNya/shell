#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

// Executes a specific function, parsing the input and running execvp
int exec_line(char *input);
// Handles all commands given by main()
void runs_command(char *scpy);
// Removes extraneous white spaces from str
void trim(char *str);
// Returns the cwd in a presentable string
char * wrkdir();

int main() {
  int status;
  char s[1024];
  
  char *s1;
  char *s2;
  char *commands[1024];
  char *cwd;
  
  while(1) {
   cwd = wrkdir();
    /*
    getcwd( cwd, sizeof(cwd) );  //keep if we want to display the current working directory
    */
    printf("%s ^_^ : ", cwd);
    fgets(s,sizeof(s),stdin);
    
    int i=0;
    for(;s[i]!='\n';i++){
    }
    s[i]=0;
    
    char *scpy;
    scpy=(char*)malloc(1024);
    strcpy(scpy,s);
    
    // this part parses the input by ";" and puts each command in commands
    s1 = s;
    for(i=0;s1;i++){
      s2 = strsep(&s1,";");
      commands[i]=s2;
    }
    commands[i] = NULL;

    for(i=0;scpy;i++) {
      scpy = commands[i];
      if(!scpy) {
	break;
      }
      runs_command(scpy);
    } 
  }
}

void runs_command(char *scpy) {
  trim(scpy);
  char s[1024];
  char *first_arg;
  strcpy(s,scpy);
  
  first_arg = strsep(&scpy," ");
  //Exit
    if(strcmp("exit",first_arg) == 0 || strcmp("Exit",first_arg) == 0) {
      printf("Exiting\n");
      exit(0);
    } 

    //CD
    else if (strcmp("cd",first_arg) == 0) {
      chdir(scpy);
      char direct[1024];
      getcwd(direct,sizeof(direct));
      printf("Current Directory: %s\n",direct);
    }
    //> and >>
    else if(strchr(s,'>')) {
      char *scpy2 = (char *)malloc(1024);
      strcpy(scpy2, s);
      char *first_cmd = (char *)malloc(1024);
      first_cmd = strsep(&scpy2, ">");
 
      char *second_cmd = (char *)malloc(1024);
      strcpy(second_cmd, scpy2);
      trim(first_cmd);
      trim(second_cmd);
      //printf("first :%s:\n", first_cmd);
      //printf("secnd :%s:\n", second_cmd);
      
      int f, fd, s, temp, status;
      char *tmp = (char *)malloc(1024);
      f = fork();
      
      if( !f ){
	if( strchr(second_cmd, '>') ){ //if original cmd was COMMAND >> FILE
	  tmp = strsep(&second_cmd, ">");
	  trim(second_cmd);
	  fd = open(second_cmd,O_CREAT | O_WRONLY | O_APPEND ,0644);
	} else{ 
	  fd = open(second_cmd,O_CREAT | O_WRONLY | O_TRUNC, 0644);
	}
	temp = dup(STDOUT_FILENO);
	dup2(STDOUT_FILENO, temp);
	dup2(fd, STDOUT_FILENO);
	exec_line(first_cmd);
	close(fd);
	close(temp);
	exit(0);
	
      } else {
	wait(NULL);
      }
      /*
      free(&scpy2);
      free(&first_cmd);
      free(&second_cmd);
      free(&tmp);
      */
    }
    //<
    else if(strchr(s,'<')) {
      char *scpy2 = (char *)malloc(1024);
      strcpy(scpy2, s);
      char *first_cmd = (char *)malloc(1024);
      first_cmd = strsep(&scpy2, "<");
      
      char *second_cmd = (char *)malloc(1024);
      strcpy(second_cmd, scpy2);
      trim(first_cmd);
      trim(second_cmd);
      //printf("first :%s:\n", first_cmd);
      //printf("secnd :%s:\n", second_cmd);
   
      int f, fd, s, temp, status;
      f = fork();
      
      if( !f ){
	fd = open(second_cmd, O_RDONLY , 0644);
	temp = dup(STDIN_FILENO);
	dup2(STDIN_FILENO, temp);
	dup2(fd, STDIN_FILENO);
	exec_line(first_cmd);
	close(fd);
	close(temp);
	exit(0);
      } else {
	wait(NULL);
      } 

    }
    // | pipes
    else if(strchr(s,'|')) {
    	if(fork() == 0) {
    		int stdin_copy;
    		char *s1;
    		char *s2;
    		char *commands[1024];
    		char *jscpy = malloc(1024);
    		int i;
    		int in = 0;
    		int out;
    		s1 = s;
    		for(i=0;s1;i++){
    			s2 = strsep(&s1,"|");
    			trim(s2);
    			commands[i]=s2;
    		}
    		commands[i] = NULL;

    		int fd[2];

    		for(i = 0; commands[i+1]; i++) {
    			pipe(fd);
    			out = fd[1];
    			if(fork() == 0) {
    				if (in != 0) {
    					dup2(in, 0);
    					close(in);
    				}

    				if (out != 1) {
    					dup2(out, 1);
    					close(out);
    				}
    				exec_line(commands[i]);
    			}
    			close(fd[1]);
    			in = fd[0];
    		}
    		if (in != 0) {
    			dup2(in,0);
    		}
    		exec_line(commands[i]);
    	} else {
    		wait(NULL);
    	}
    	
    }
    // ALL OTHER COMMANDS
    else {
      
      int f = fork();
      if(f == 0) {
	exec_line(s);
	exit(0);
      }
      else {
	wait(NULL);
      }
      
    }
}

int exec_line(char *s) {
  //printf("exec line: %s\n",s);
  trim(s);
  char* string2;
  char *array[256];
  
  int i=0;
  
  if( strchr(s, '\n') ){
    for(;s[i]!='\n';i++){
    }
    s[i]=0;
  }
  
  char* string1=s;
  
  for(i=0;string1;i++){
    string2 = strsep(&string1," ");
    array[i]=string2;
  }
  array[i]=0;
  execvp(array[0],array);
  return 0;
}

void trim(char *str) {
  int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (str[begin] == ' ')
        begin++;

    while ((end >= begin) && str[end] == ' ')
        end--;

    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0';
}

char * wrkdir(){
  char *path = (char *)malloc(512);
  path = getcwd( path, 512 );
  char * temp = (char *)malloc(512);
  temp = getcwd(temp, sizeof(temp) );
  int branches = 0;
  while( (temp = strsep(&path, "/")) ){
    branches++;
  }
  if( branches <= 3 ){
    path = getcwd(path, sizeof(path) );
  } else {
    path = getcwd(path, sizeof(path) );
    int i = 0;
    for(i; i< branches-3 ; i++){
      temp = strsep(&path, "/");
    }
  }
  return path;
}
