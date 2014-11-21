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
    //printf("MY ASS\n");
    char input[256];
    //printf("MY ASS\n");
    fgets(input,sizeof(input),stdin);
    
    //printf("MY ASS\n");
    char ** semicolon_parsed = parse_string(input,";");

    
    //printf("MY ASS\n");
    int e=0;
    printf("%d/%d\n",sizeof(semicolon_parsed),sizeof(semicolon_parsed[0]));
    int sizeof_semicolon_parsed = sizeof(semicolon_parsed)/sizeof(semicolon_parsed[0]);
    for (;e<=sizeof_semicolon_parsed+1;++e)
      printf("res[%d] = |||%s|||, with length %lud chars, there are %d items\n",e, semicolon_parsed[e],sizeof( semicolon_parsed[e])/sizeof(char), sizeof_semicolon_parsed);
    printf("\n\n\n");
    
    int i = fork();
    if (i) {
      wait();
    }
    else {
      int j=0;
      
      for (;j<=sizeof_semicolon_parsed+1;++j) {
	printf("res[%d] = |||%s|||, with length %lu chars\n",j, semicolon_parsed[j],sizeof( semicolon_parsed[j])/sizeof(char));
	char ** command = parse_string(semicolon_parsed[j]," ");
	int z = sizeof(command)/sizeof(command[0]);
	int y;
	for (;y<=z;++y) 
	  printf("\tres[%d] = |||%s|||, with length %lu chars\n",y, command[y],sizeof(command[y])/sizeof(char));
	
	execvp(command[0],command);
	free(command);
      }
      free(semicolon_parsed);
      return 0;
    }
  }
}
