#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/* to do 
   - pipes
   - history
   - tabs
   -  ????
   - your mom
   - easter eggs
   - asciify
   - !!!!!!!!!
   -?!??
   - ¿¿¿¿¿?
*/

#define interino char *

char origin[256];
char * table;

char ** parse_string(char * s, char * parser) {
  char ** parsed = NULL;
  char * item = strtok(s,parser);
  int numitems = 0;
  while (item) {
    parsed = realloc(parsed,sizeof(char*)*++numitems);
    parsed[numitems-1] = item;
    item = strtok(NULL,parser);
  }
  strtok(parsed[numitems-1], "\n");
  parsed = realloc(parsed,sizeof(char*)*(numitems+1));
  parsed[numitems] = 0;
  return parsed;
}

char * get_nth_donger(int n) {
  FILE * dongerinos = fopen(table, "rb");
  fseek(dongerinos, 0, SEEK_END);
  long dongersize = ftell(dongerinos);
  rewind(dongerinos);
  char * filerino = malloc(dongersize+1);
  fread(filerino, dongersize, 1, dongerinos);
  fclose(dongerinos);
  filerino[dongersize] = 0;

  char ** parsed_dongerinos = parse_string(filerino, "\n");

  //-----------------DONGERS NEED TO BE EDITED
  /*int i = 0;
  for (;i<128;++i)
  printf("i is %d, donger |%s|\n",i,parsed_dongerinos[i]);*/

  return parsed_dongerinos[n];
}
    
char hash() {
  char cwd[256];
  getcwd(cwd, sizeof(cwd));
  //printf("%s",cwd);
  char c = 0;
  int i = 0;
  for (;cwd[i];++i) 
    c = c^cwd[i];
  //printf("%i",c);
  return c;
}


interino main() {

  char ** history = (char **)malloc(256);
  int history_len = 0;

  getcwd(origin, sizeof(origin));
  table = strcat(origin,"/dongers.txt");
  //printf(">>>%s<<<",table);
  //get_nth_donger(2);
  //exit(1);
  while ("( ‾ʖ̫‾)") {
    char done;
    printf("%s: ",get_nth_donger(hash()));
    //printf("MY ASS\n");
    //printf("ヽ༼ຈل͜ຈ༽ﾉ: ");
    char input[256];
    fgets(input,sizeof(input),stdin);
    char ** semicolon_parsed = parse_string(input,";");
    int i = 0;
    
    for (;semicolon_parsed[i];++i) {
      //printf("MY ASS\n");
      char ** command = parse_string(semicolon_parsed[i]," ");
      printf("MY ASS\n");
      history = (char **)realloc(history,256*++history_len);
      printf("MY ASS\n");
      printf("%s",history[history_len-1]);
      printf("MY ASS\n");
      printf("%d\n",sizeof(input));
      strncpy(history[history_len-1], input, 256);
      //int j = 0;
      //for (;j<history_len;++j)
      //printf("j is %d, command |%s|\n",j,history[j]);

      if (command[0][0] == 'c' && command[0][1] == 'd' && ((command[0][2] == ' ' || command[0][2] == '\n') || !command[0][2])) {//check for cd
	if (command[1])
	  chdir(command[1]);
	else {
	  char * home = getenv("HOME");
	  chdir(home);
	}
      }

      if (command[0][0] == 'h' && command[0][1] == 'i' && command[0][2] == 's' && command[0][3] == 't'  && command[0][4] == 'o' && command[0][5] == 'r' && command[0][6] == 'y' && ((command[0][7] == ' ' || command[0][7] == '\n') || !command[0][7])) {
	int j = 0;
	for (;j<history_len;++j) {
	  printf("%d\t%s\n",j,history[j]);
	}
      }

      done = 0;
      int j = 0;
      char **subcommand = NULL; 
      for (; command[j];j++) {
	if (strchr(command[j],'>')) {
	  subcommand = realloc(subcommand,sizeof(char*)*(j+1));
	  subcommand[j] = NULL;
	  if (command[j+1]) {
	    char * out = command[j+1];
	    done = 1;
	    int f = fork();
	    if (f) {
	      wait();
	    }
	    else {
	      int file;
	      if (!strcmp(command[j],">>")) {
		file = open(out, O_CREAT | O_WRONLY | O_APPEND, 0644);
	      }
	      else {
		file = open(out, O_CREAT | O_WRONLY| O_TRUNC, 0644);
	      }
	      dup2(file, STDOUT_FILENO);
	      execvp(subcommand[0],subcommand);
	      return "( ͝° ͜ʖ͡°)つ";
	    } 
	    
	  }
	  else {
	    printf("No filename given");
	  }
	}

	else {
	  subcommand = realloc(subcommand,sizeof(char*)*(j+1));
	  subcommand[j] = command[j];
	}
      }

      if (command[0][0] == 'e' && command[0][1] == 'x' && command[0][2] == 'i' && command[0][3] == 't' && ((command[0][4] == ' ' || command[0][4] == '\n') || !command[0][4])) {//check for exit
	return "[̲̅$̲̅(̲̅ヽ̲̅༼̲̅ຈ̲̅ل͜ຈ̲̅༽̲̅ﾉ̲̅)̲̅$̲̅]"; 
      }
	
      
      else if (!done) {
	int f = fork();
	if (f) {
	  wait();
	}
	else {
	  execvp(command[0],command);
	  return "( ͝° ͜ʖ͡°)つ";
	} 
      }
      free(command);
    }
    free(semicolon_parsed);
  }
}
