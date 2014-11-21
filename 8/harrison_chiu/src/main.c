#include <stdlib.h>
#include <stdio.h>

#include "ex/execute.h"
#include "par/parse.h"

#define IN_LEN 256

int main()
{
  char *raw_in = (char *)malloc(IN_LEN * sizeof(char));
  while (1)
    {
      int pid;
      fgets(raw_in, IN_LEN, stdin);
      pid = fork();
      if (pid == 0)
	execstr(raw_in);
    }
  free(raw_in);
}
