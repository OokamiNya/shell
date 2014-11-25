#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "execute.h"

int execstr(char *str)
{
  str = strsep(&str, "\n"); //gets rid of trailing newline-- thanks to Jerry Coffin on stackoverflow.com

  //break the input into flags
  int numflags = strcount(str, " ") + 1;
  char **in = tokify(&str, " ", numflags);
  
  //show command and flags
  //printcomm(in, numflags); //come back to this line for debugging

  //execute stuff
  execvp(in[0], in);
}

int printcomm(char **in, int n)
{
  int index;
  int count = 0;
  count += printf("command: (%s)\n", in[0]);
  count += printf("flags:");
  for (index = 1; index < n; index++)
    {
      count += printf(" (%s)", in[index]);
    }
  count += printf("\n");
  return count;
}

static int strcount(char sup[], char *sub)
{
  if (!sup)
    return -1;
  int count = 0;
  while (sup = strstr(sup, sub))
    {
      count++;
      sup++;
    }
  return count;
}

static char **tokify(char **str, char *delim, int n)
{
  char **out;
  out = malloc(n * sizeof(char*));
  int index;
  for (index = 0; index < n; index++)
    {
      out[index] = strsep(str, delim);
    }
  return out;
}
