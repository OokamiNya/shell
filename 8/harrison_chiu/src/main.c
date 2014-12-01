#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

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
      /* char **in = &raw_in; */
      /* char *delim = malloc(1 * sizeof(char)); */
      /* int count; */
      /* for (count = 0; count < num_cmds; count++) */
      /* 	{ */
      /* 	  printf("%s\n", strsep_save(in, DELIMS, delim)); */
      /* 	} */
      /* free(delim); */
      command_t **cmds = (command_t **)malloc(num_cmds * sizeof(command_t *));
      parse(&raw_in, cmds, num_cmds);

      //exec functions
      int count;
      for (count = 0; count < num_cmds; count++)
      	{
      	  int pid = fork();
      	  if (pid == 0)
      	    {
	      umask(0000);
	      printf("cmd: (%s)\nin:  (%s)\nout: (%s)\n", cmds[count]->execstr, cmds[count]->f_in, cmds[count]->f_out);
	      if (cmds[count]->f_in)
		dup2(open(cmds[count]->f_in, O_RDONLY | O_CREAT, 0644), STDIN_FILENO);
	      if (cmds[count]->f_out)
		dup2(open(cmds[count]->f_out, O_WRONLY | O_CREAT | O_TRUNC, 0644), STDOUT_FILENO);
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
