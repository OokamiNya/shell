#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


/* to do 
   - hash table + custom dongerinos for directory
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

#define interino char *

interino main() {
  while ("( ‾ʖ̫‾)") {
   
    //printf("MY ASS\n");
    printf("ヽ༼ຈل͜ຈ༽ﾉ: ");
    char input[256];
    fgets(input,sizeof(input),stdin);
    char ** semicolon_parsed = parse_string(input,";");
    int i = 0;
    
    for (;semicolon_parsed[i];++i) {
      char ** command = parse_string(semicolon_parsed[i]," ");
      
      if (command[0][0] == 'c' && command[0][1] == 'd' && ((command[0][2] == ' ' || command[0][2] == '\n') || !command[0][2])) {//fuck strstr
	if (command[1])
	  chdir(command[1]);
	else {
	  char * home = getenv("HOME");
	  chdir(home);
	}
      }
      if (command[0][0] == 'e' && command[0][1] == 'x' && command[0][2] == 'i' && command[0][3] == 't' && ((command[0][4] == ' ' || command[0][4] == '\n') || !command[0][4])) {
	return "[̲̅$̲̅(̲̅ヽ̲̅༼̲̅ຈ̲̅ل͜ຈ̲̅༽̲̅ﾉ̲̅)̲̅$̲̅]"; 
      }
	
      
      else {
	int j = fork();
	if (j) {
	  wait();
      }
	else {
	  execvp(command[0],command);
	  free(command);
	  return "( ͝° ͜ʖ͡°)つ";
	}      
      }
      
    }
    free(semicolon_parsed);
  }
}
*/
