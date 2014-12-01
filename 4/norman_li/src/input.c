#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/input.h"

// Get Input Function
/* Explanation:
   This function grabs the input from the terminal/console
   character by character until it reaches EOF (End of File).
*/
/* Arguements: None
   Returns (void)
*/
char *get_input()
{
  int size = 0;
  char *input = (char *) calloc(1, size + 1);

  char ch = getchar();

  while ( strncmp(&ch, "\n", 1) ) 
  {
    size++;
    input = (char *) realloc(input, size + 1);
    strncat(input, &ch, 1);
    ch = getchar();
  }

  return input;
}
