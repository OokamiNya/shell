#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "parse.h"

int parse(char **in, command_t *cmds[], int num_cmds)
{
  int count;
  int pipin = 0;
  for (count = 0; count < num_cmds; count++)
    {
      char *delim = (char *)malloc(1 * sizeof(char));

      cmds[count] = (command_t *)malloc(1 * sizeof(command_t));
      cmds[count]->execstr = strsep_save(in, DELIMS, delim);

      //setting stdin
      if (pipin)
	{
	  cmds[count]->f_in = (char *)malloc(strlen(TEMP_FILE) * sizeof(char));
	  strcpy(cmds[count]->f_in, TEMP_FILE);
	  pipin = 0;
	}
      else if (cmds[count]->f_in = strchr(cmds[count]->execstr, F_IN))
	{
	  cmds[count]->f_in++[0] = 0;
	}

      //setting stdout
      if (*delim == '|')
	{
	  cmds[count]->f_out = (char *)malloc(strlen(TEMP_FILE) * sizeof(char));
	  strcpy(cmds[count]->f_out, TEMP_FILE);
	  cmds[count]->piped = 1;
	  pipin = 1;
	}
      else if (cmds[count]->f_out = strchr(cmds[count]->execstr, F_OUT))
	{
	  cmds[count]->f_out++[0] = 0;
	}

      free(delim);
    }
}

char *strsep_save(char **stringp, char *delims, char *found)
{
  char *string = *stringp;
  int i_string;
  for (i_string = 0; string[i_string]; i_string++)
    {
      int i_del;
      for (i_del = 0; delims[i_del]; i_del++)
      	{
      	  if (string[i_string] == delims[i_del])
      	    {
      	      char *tok = string;
      	      *found = delims[i_del];
      	      string[i_string++] = 0;
      	      *stringp = &string[i_string];
      	      return tok;
      	    }
      	}
    }
  return string;
}

int count_delims(char *in, char *delims)
{
  int i_in, i_del, count = 0;
  for (i_in = 0; in[i_in]; i_in++)
    {
      for (i_del = 0; delims[i_del]; i_del++)
	{
	  if (in[i_in] == delims[i_del])
	    {
	      count++;
	      break;
	    }
	}
    }
  return count;
}
