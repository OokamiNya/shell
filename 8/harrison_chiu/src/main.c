#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ex/execute.h"
#include "par/parse.h"
#include "out/output.h"

#define IN_LEN 256

int main()
{
  char *raw_in = (char *)malloc(IN_LEN * sizeof(char));
  char *prompt = (char *)malloc(IN_LEN * sizeof(char));
  while (printf("%s", create_prompt(prompt)))
    {
      //process input
      fgets(raw_in, IN_LEN, stdin);
      raw_in = strsep(&raw_in, "\n");

      //parse functions
      int num_cmds = count_delims(raw_in, DELIMS) + 1;
      command_t **cmds = (command_t **)malloc(num_cmds * sizeof(command_t *));
      parse(raw_in, cmds, num_cmds);

      //exec functions
      int count;
      for (count = 0; count < num_cmds; count++)
      	{
	  printf("COMMAND: %s\n", cmds[count]->execstr);
      	  int pid = fork();
      	  if (pid == 0)
      	    {
      	      execstr(cmds[count]->execstr);
      	    }
      	  else
      	    {
      	      wait(NULL);
      	    }
      	}
    }
  free(raw_in);
  free(prompt);
}
