#include <stdio.h>
#include <stdlib.h>

#include "../include/control.h"
#include "../include/prompt.h"
#include "../include/list.h"
#include "../include/parse.h"
#include "../include/input.h"

// Control Flow Function
/* Explanation:
   The "Control Center" of the whole shell.
   This function controls the flow of the program
*/
/* Arguements: None
   Returns (void)
*/
void control_flow()
{
  char *input = 0;
  char *formatted_cmd = 0;
  struct node* commands = 0;

  // Print Prompt
  print_prompt();

  // Flush the Stream and Get input
  fflush(stdin);
  input = get_input();

  // Formats the Input
  formatted_cmd = remove_extra(input);

  // Get Linked List of Commands
  commands = split_on_semicolon(formatted_cmd);

  // Run commands
  run_commands(commands);

  // Cleanup
  free_list(commands);
  free(input);
  free(formatted_cmd);
}
