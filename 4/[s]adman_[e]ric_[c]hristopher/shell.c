#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

//just incase lmao
#include <errno.h>

int currentPID;
//counts the num of substr in string
int countchar(char* str,char substr){
  int ans=0;
  int index=0;
  while(str[index]){
    if (str[index++]==substr)
      ans++;
  }
  return ans;
}
int wrap_with_semicolons_LOL(char* stuff){
  char* semicolon_buffer=calloc(256,sizeof(char));
  while(semicolon_buffer=strsep(&stuff,";")){
    doPipeStuff(semicolon_buffer);
  }
  return 0;
}
//pipe accepts the whole line of args, 
int doPipeStuff(char* arg){
  char* split_buffer=calloc(256,sizeof(char));
  char* arg_buffer=calloc(256,sizeof(char));
  
  int stdin_buffer=dup(STDIN_FILENO),
    stdout_buffer=dup(STDOUT_FILENO),
    file_in,
    file_out,
    commands=countchar(arg,124)+1,/*124 is pipe*/
    command_index=0;
  for (;command_index<commands;command_index++){
    int boolleft=countchar(split_buffer,'<');
    int boolright=countchar(split_buffer,'>');
    split_buffer=strsep(&arg,"|");
    int i=0;
    char** addresses= calloc(256,sizeof(char*));
    while (arg_buffer=strsep(&split_buffer," "))
      addresses[i++]=arg_buffer;
    if (! fork()){
      if(command_index){//if not at first command
	file_in = open("piped",O_RDONLY);
	dup2(file_in,STDIN_FILENO);
	close(file_in);
      }else if (boolleft==1){
	printf("ran\n");
	char* split_buffer2 = calloc(256,sizeof(char));
	split_buffer2 = strsep(&split_buffer,"<");
	split_buffer2 = strsep(&split_buffer,"<");
	file_in = open(split_buffer2,O_RDONLY);
	dup2(file_in,STDIN_FILENO);
	close(file_in);
      }
      if(commands-command_index-1){//if not at last command
	file_out = open("piped",O_WRONLY|O_CREAT|O_TRUNC);
	dup2(file_out,STDOUT_FILENO);
	close(file_out);
      }else if (boolright == 1){
	printf("ran\n");
	char* split_buffer2 = calloc(256,sizeof(char));
	split_buffer2 = strsep(&split_buffer, ">"); 
	split_buffer2 = strsep(&split_buffer, ">");
	//strsep for spaces
	file_out = open(split_buffer2 , O_WRONLY|O_CREAT|O_TRUNC);
	dup2(file_out,STDOUT_FILENO);
	close(file_out);
      }
      
      execvp(addresses[0],addresses);
    }else{
      wait(-1);
      //reset stdin/out
      dup2(stdin_buffer,STDIN_FILENO);
      dup2(stdout_buffer,STDIN_FILENO);
    }
  }
  return 0;
}
static void sighandler(int signo){
  if (signo == SIGINT && !getpid()){
    printf("caught interrupt\n");
    printf("%d\t%d\n", getpid(), currentPID);
    kill(currentPID, 9);
    currentPID = getpid();
  }
}


int main(){
  signal(SIGINT, sighandler);
  //printf("%d\n", getpid());
  umask(0000);

  char* args=calloc(256,sizeof(char));
  char** addresses=calloc(256,sizeof(char*));
  
  char* cwd=calloc(256,sizeof(char));  
  getcwd(cwd,256);
  int status;
  while (1){
    printf("(ﾉ◕ヮ◕)ﾉ*:・ﾟ✧");
    fgets(args,256,stdin);
    args = strsep(&args,"\n");
    char* p;
    for(p=args;*p;++p) *p= tolower(*p);
    //exit
    if (! strncmp(args,"exit",4))
      exit(0);
    //cd
    else if (! strncmp(args,"cd",2)){
      execlp("cd","cd",args+3,NULL);
      chdir(args+3);
      getcwd(cwd,256);
      printf("%s\n", cwd);
    }else if(args[0]){//prevents empty lines
      currentPID = getpid();
      wrap_with_semicolons_LOL(args);
      //printf("Shell PID: %d\tCurrent Process PID: %d\n", getpid(), currentPID);
    }else{
      wait(-1);
      //printf("2nd :Shell PID: %d\tCurrent Process PID: %d\n", getpid(), currentPID);
      
    }
  }
  //ls | grep poop > swag.txt
  remove("piped");
  return 0;
}
