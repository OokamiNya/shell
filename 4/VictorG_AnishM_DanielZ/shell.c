#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
//more includes
void run(char * input);

void myexec(char *input){
  int i = 0;
  char *singlearg;
  char * args[10];
  while ((singlearg = strsep(&input, " ") )) {
    args[i] = singlearg;
    i++;
  }
  args[i] = 0;
  int f = fork();
  if ( !f ) {
    execvp(args[0], args);
    exit(0);
  }
  else {
    wait( &f );
      f = 0;
  }
}

void piping(char * args[]){
  int check1,check2,check3;
  int i = fork();
  if (!i){
    check1 = open("check.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);
    dup2(check1,STDOUT_FILENO);
    execlp(args[0],args[0],NULL);
  }
  else{
    check2 = wait(&check3);
    check1 = open("check.txt",O_RDONLY);
    dup2(check1,STDIN_FILENO);
    execlp(args[2],args[2],NULL);
  }
}

void semisep(char *s){
  int i=0;
  char * raw;
  if (strstr(s,"\n")){
    raw=strsep(&s,"\n");
  }
  else{
    raw=s;
  }
  char * singlecommand;
  while ((singlecommand=strsep(&raw,";") )) {
    int f=fork();
    if (!f)
      run(singlecommand);
    wait(NULL);
  }
}



char ** separate(char *s){
  char *pos;
  int i=0;
  if ((pos=strchr(s, '\n')) != NULL)
    *pos = '\0';
  char *s1=s;
  char **s2=(char**)(malloc(strlen(s)*strlen(s)*sizeof(char)+sizeof(char)));

  for (i=0;s[i];i++){
    s2[i] = (char *)(malloc(strlen(s)*sizeof(char)));
    s2[i] = strsep(&s1, " ");
  }

  s2[i]=NULL;
  return s2;
}
void redirout(char** args, char * dest) {
  int fd;
  fd = open(dest, O_WRONLY | O_TRUNC | O_CREAT, 0666);
  dup2(fd,STDOUT_FILENO);
  execvp(args[0], args);
  return;
}
void redirin(char** args, char * source){
  int fd;
  fd = open(source, O_RDONLY);
  dup2(fd,STDIN_FILENO);
  execvp(args[0], args);
  return;
}

void redirpipe(char** args, char ** args2) {
  pid_t child = fork();
  int check2;
  int w;
  if (child==0){
    redirout(args,"check.txt");
  }  
  else {
    check1 = wait(&check2);
    int i = 0;
    for (i;i<8;i++){
      args2[i]=args2[i+1];
    }
    args2[8]='\0';
    redirin(args2,"check.txt");
  }
}

void run(char *input){
  if (!strcmp(input,"\n")){//so you can hit enter with no commands
    return;
  }
  else if (strchr(input, ';')){
    semisep(input);
    return;
  }
  else if (strstr(input, "<")){
    char *input2=input;
    char ** args;
    args=separate(strsep(&input2,"<"));
    char * raw;
    if (strstr(input2,"\n")){
      raw=strsep(&input2,"\n");
    }
    else{
      raw=input2;
    }
    int f=fork();
    if (!f)
      redirin(args,raw);
    return;
  }
  else if (strstr(input, ">")){
    char *input2=input;
    char ** args;
    args=separate(strsep(&input2,">"));
    char * raw;
    if (strstr(input2,"\n")){
      raw=strsep(&input2,"\n");
    }
    else{
      raw=input2;
    }
    int f=fork();
    if (!f)
      redirout(args,raw);
    return;
  }
  else if (strstr(input, "|")){
    char * input2=input;
    char * raw;
    if (strstr(input2,"\n")){
      raw=strsep(&input2,"\n");
    }
    else{
      raw=input2;
    }
    char ** args;
    char ** args2;
    args=separate(strsep(&raw,"|"));
    printf("%s",raw);
    args2=separate(raw);
    int f=fork();
    if(!f)
      redirpipe(args,args2);
    return;
  }
  char **inputA=malloc(strlen(input)*strlen(input)*sizeof(char)+sizeof(char));
  inputA=separate(input);
  if (!strcmp(inputA[0],"cd")){
    if(inputA[1]){
      if (inputA[1][0]=='~'){
	//printf("here\n");
      	char a;
      	char * b=(char *) malloc(sizeof(char)*strlen(inputA[1]));
      	strcpy(b,inputA[1]);
      	a=b[1];
      	//printf("This is b: %s\n",b);
      	int c=1;
      	inputA[1][0]=b[c];
	for (c=2;!inputA[1][c];c++){
      	  inputA[1][c]=b[c];
      	  //printf("%c\n",b[c]);
        }
      	//printf("%s\n",inputA[1]);
      	char *env;
      	strcpy(env,getenv("HOME"));
      	chdir(strcat(env,inputA[1]));
      	//free(env);
      	free(b);
      	free(inputA);
      	return;
      }
      //printf("hey\n");
      int i = chdir(inputA[1]);
      if (i!=0){
	printf("%s is not a directory.",inputA[0]);
	free(inputA);
	return;
      }
    }
    else{
      printf("%s\n",getenv("HOME"));
      chdir(getenv("HOME"));
      free(inputA);
      return;
    }
  }
    if (!strcmp(inputA[0],"exit")){
      int z=fork();
      if (z==0){
	printf("Thanks for using our shell!\n");}
      free(inputA);
      exit(0);
      return;
    }
    //printf("heyhey\n");
    else{
      int f=fork();
      if (!f)
	execvp(inputA[0],inputA);
      free(inputA);
      return;
    }
}




int main(){
  char s[1024];
  int f=1;

  while(1){
    printf("==|==<(x_x)>===>:");
    fgets(s,1024,stdin);
    //f=fork();
    run(s);
    //return 0;
    wait(NULL);
  }

  //char damn[256]="ls;echo hello\n";
  //semisep(damn);
  return 0;
}
