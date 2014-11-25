#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

int main() {
  while ("( ‾ʖ̫‾)") {
    //printf("MY ASS\n");
    printf("ヽ༼ຈل͜ຈ༽ﾉ: ");
    char input[256];
    fgets(input,sizeof(input),stdin);
    char ** semicolon_parsed = parse_string(input,";");
    int i = 0;
    for (;semicolon_parsed[i];++i) {
      char ** command = parse_string(semicolon_parsed[i]," ");
      if (command[0][0] == 'c' && command[0][1] == 'd') {//fuck strstr
	if (command[1])
	  chdir(command[1]);
	else {
	  char * home = getenv("HOME");
	  chdir(home);
	}
      }
      else {
	int j = fork();
	if (j) {
	  wait();
      }
	else {
	  
	  execvp(command[0],command);
	  free(command);
	  return 0;
	}      
      }
    }
    free(semicolon_parsed);
  }
}
