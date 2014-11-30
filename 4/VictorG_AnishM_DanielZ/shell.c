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
  char *singlearg; //holds the piece separated off from x
  char * args[10]; //arg array
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

void semisep(char *s){
  //char *pos;
  //printf("%s",s);
  int i=0;
  //printf("%s\n",s);
  //if ((pos=strchr(s, '\n')) != NULL)
  //  *pos = '\0';
  //printf("%s",s);
  char * raw;
  if (strstr(s,"\n")){
    raw=strsep(&s,"\n");
  }
  else{
    raw=s;
  }
  //char **s2=(char**)(malloc(strlen(s)*strlen(s)*sizeof(char)+sizeof(char)));
  char * singlecommand;
  //printf("yo\n");

  //printf("%s",raw);
  while ((singlecommand=strsep(&raw,";") )) {
    //printf("%s\n",singlecommand);
    //int f=fork();
    //if (!f){
    myexec(singlecommand);
    //}
    //wait(&f);
  }
  /*
  for (i=0;s1[i];i++){
    s2[i] = (char *)(malloc(strlen(s)*sizeof(char)));
    //printf("s2 :%s:\n", s2[i]);
    //printf("hey\n");
    s2[i]=strsep(&s1, ";");
    //strsep(&s1,";");
    printf("%s\n",s2[i]);
  }
  //printf("s1:%s\n",s1);
  //s2[i]=(char *)malloc(25*sizeof(char));
  //s2[i]=s1;
  //i++;
  s2[i]=(char *)malloc(25*sizeof(char));
  s2[i]=NULL;
  int b;
  for(b=0;s2[b];b++){
    //int f;
    printf("%s\n",s2[b]);
    //f=fork();
    if(!fork()){
      run(s2[b]);
      return;
    }
    printf("%d\n",b);
    //free(s2);
    
  }
  printf("out\n");
  //free(s2);
  return;
  //s2[i-1]=

  //printf("hey\n");
  //return s2;
  */
}



char ** separate(char *s){
  char *pos;
  int i=0;
  //printf("%s\n",s);
  if ((pos=strchr(s, '\n')) != NULL)
    *pos = '\0';
  char *s1=s;
  char **s2=(char**)(malloc(strlen(s)*strlen(s)*sizeof(char)+sizeof(char)));
  //printf("yo\n");
  //printf("%s\n",s);

  for (i=0;s[i];i++){
    s2[i] = (char *)(malloc(strlen(s)*sizeof(char)));
    //printf("s2 :%p:\n", s2[i]);

    s2[i] = strsep(&s1, " ");
    //printf("%s\n",s2[i]);
  }

  //s2[i-1]=
  //s2[i]=(char *)malloc(25*sizeof(char));
  s2[i]=NULL;
  //printf("hey\n");
  return s2;
}
void run(char *input){
  char **inputA;//=malloc(strlen(input)*strlen(input)*sizeof(char)+sizeof(char));
  if (!strcmp(input,""))//so you can hit enter with no commands
    return;
  if (strchr(input, ';')){
    semisep(input);
    return;
  }
  inputA=separate(input);
  //char* problemChild=(char *)malloc(strlen(inputA[0])*sizeof(char));
  //problemChild=inputA[0];
  //printf("%s\n",problemChild);
  

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
	printf("%s\n",inputA[1]);
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
    printf("%s\n",getenv("HOME"));
    chdir(getenv("HOME"));
    free(inputA);
    return;
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
 
