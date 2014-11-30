#include "program.h"

char home[256] = "/";

void process(char * start){
  //site of user input processing
  while (start){
    execute(strsep(&start, ";"));      
  }
}


void execute(char * start){
  //execute: Takes a char pointer start representing the beginning f a String
  //Splits the string arguements and runs them with a child process and execvp
  //Returns nothing
  //printf("%s\n",start);
  if( ! strncmp(start,"HOME=",5))
    set_home(start);
  char * args[10];
  int x = 0;
  char * y;
  int redir = 0;
  while (start){
    y = strsep(&start, " ");
    if(*y != 0){
      if (! strcmp(">>",y))
	  redir = 4;
	else if( ! strcmp("<<",y))
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
    //printf("%d", redir);
    redirect(args, redir);
  }else{
    if (! (strcmp("cd",args[0]) && strcmp("exit",args[0])))
      normal_process(args);
    else{
      child_process(args);
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
  int f = fork();
  if (!f){
    //printf("running: %s %s\n",args[0],args[1]);
    //execl("/bin/ls","ls","-l",NULL);
    execvp(args[0],args);
  }
  else{
    wait(&f);
  }
}

void redirect(char * args[], int redir){
	int c;
	int i = fork();
	if(!i){
		if (redir == 4){
			c = open(args[2], O_CREAT | O_WRONLY | O_APPEND, 0644);
			dup2(c, STDOUT_FILENO);
			execlp(args[0], args[0], NULL);
			close(c);
		}
		else if (redir == 3){
			c = open(args[2], O_RDWR, 0777);
			printf("%d\n",c);
			dup2(c, STDIN_FILENO);
			execvp(args[0], STDIN_FILENO);
			close(c);
		}
		else if (redir == 2){
			c = open(args[2],O_RDWR,0777);
			dup2(c,STDIN_FILENO);
			close(c);
			execlp(args[0],args[0],NULL);
		}else{
			c = open(args[2], O_CREAT | O_WRONLY, 0644);
			dup2(c, STDOUT_FILENO);
			execlp(args[0], args[0], NULL);
			close(c);
		}
 	}else{
		wait(&i);
		redir = 0;
	}
}

int main(int argc, char *argv[]){
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
    printf("%s/%s: input: ",args[x-2],args[x-1]);
    char input[256];
    fgets(input, 256, stdin);
    printf("\n");
    char * start = &input[0];
    process(start);  
    
  }
  return 0;
}
    
