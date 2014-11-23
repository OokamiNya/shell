#include <stdlib.h>
#include <stdio.h>

#include "ex/execute.h"
#include "out/output.h"

#define IN_LEN 256

int main()
{
  char *raw_in = (char *)malloc(IN_LEN * sizeof(char));
  char *prompt = (char *)malloc(IN_LEN * sizeof(char));
  while (printf("%s", create_prompt(prompt)))
    {
      int pid;
      fgets(raw_in, IN_LEN, stdin);
      pid = fork();
      if (pid == 0)
	execstr(raw_in);
      else
	wait(NULL);
    }
  free(raw_in);
}
