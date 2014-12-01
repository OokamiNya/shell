#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "../include/cd.h"

// Change Directory Function
/* Explanation:
   Changes the current directory to the one specified
   in the arguement (char *) "path". Will print an error
   if an error occurs."
*/
/* Arguements:
   -(char *): Pathname of directory to change to
   Returns (void)
*/
void cd(char *path)
{
  int status = chdir(path);

  if (status == -1)
    print_error("cd", errno);
}
