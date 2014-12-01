#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../include/error.h"

// Print Error Function
/* Explanation:
   Prints a formatted error string describing the error,
   given (char *) "command" and (int) "error".
*/
/* Arguements:
   -(char *): Command that gave the error
   -(int): Errno
*/
void print_error(char *command, int error)
{
  printf("%s: %s\n", command, strerror(error));
}
