#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parse.h"
#include "../include/list.h"

// Remove Trailing Whitespace Function
/* Explanation:
   This function removes trailing whitespace (front/back).
*/
/* Arguements:
   -(char *): The input string
   Returns (char *): The modified string
*/
char *remove_trailing_whitespace(char *input)
{
  char *start = input;
  char *end = input + strlen(input) - 1;
  int size = 0;
  char *result = (char *) calloc(1, size + 1);

  while (*input)
  {
    if ( (input != start) &&
	 (input != end))
    {
      size++;
      result = (char *) realloc(result, size + 1);
      strncat(result, input, 1);
    }
    else if ( ( strncmp(input, " ", 1) ) &&
	      ( (input == start) ||
		(input == end)))
    {
      size++;
      result = (char *) realloc(result, size + 1);
      strncat(result, input, 1);
    }

    input++;
  }

  return result;
}

// Remove Extra Function
/* Explanation:
   Given (char *) "input", this function returns a string (char *)
   without extra whitespace and without newlines.
   Example:
   remove_extra_whitespace("chicken   have  heads .\n" -> "chicken have heads ."
*/
/* Arguements:
   -(char *): String to be modified
   Returns (char *): Modified String
*/
char *remove_extra(char *input)
{
  int size = 0;
  char *output = (char *) calloc(1, size + 1);

  int next_space = 0;
  while (*input)
  {
    if (! strncmp( input, " ", 1 ) )
    {
      if (! next_space)
      {
	next_space = 1;
	size++;
	output = (char *) realloc( output, size + 1);
	strncat(output, input, 1);
      }
    }
    else
    {
      if (next_space)
      {
	next_space = 0;
	size++;
	output = (char *) realloc( output, size + 1 );
	strncat(output, input, 1);
      }
      // Ignores newlines
      else if ( strncmp(input, "\n", 1) )
      {
	size++;
	output = (char *) realloc( output, size + 1 );
	strncat(output, input, 1);
      }
    }

    input++;
  }

  return output;
}

// Split Array Function
/* Explanation:
   Given a string and a delimiter, this function
   will split the string on the delimter and return
   an array of strings.
*/
/* Arguements:
   -(char *): String to split
   -(char *): Delimiter
   Returns (char **): String Array
*/
static char **split_array(char *string, char *delim)
{
  struct node* list = 0;
  char *str = (char *) malloc( strlen(string) + 1 );
  char *temp = str;
  char *element = 0;
  char **array = 0;

  strcpy(str, string);

  element = strsep(&str, delim);

  while (element)
  {
    if ( strcmp(element, "") )
    {
      char *formatted_element = remove_trailing_whitespace(element);
      list = insert_node(list, formatted_element);
      free(formatted_element);
    }

    element = strsep(&str, delim);
  }

  array = return_string_array(list);

  free_list(list);
  free(temp);

  return array;
}

// Split List Function
/* Explanation:
   Given a string and a delimiter, this function
   will split the string on the delimter and return
   an linked list.
*/
/* Arguements:
   -(char *): String to split
   -(char *): Delimiter
   Returns (struct node*): Linked List
*/
static struct node* split_list(char *string, char *delim)
{
  struct node* list = 0;
  char *str = (char *) malloc( strlen(string) + 1 );
  char *temp = str;
  char *element = 0;

  strcpy(str, string);

  element = strsep(&str, delim);

  while (element)
  {
    if ( strcmp(element, "") )
    {
      char *formatted_element = remove_trailing_whitespace(element);
      list = insert_node(list, element);
      free(formatted_element);
    }

    element = strsep(&str, delim);
  }

  free(temp);

  return list;
}

// Arguementify Function
/* Explanation:
   Given (char *) "command", which is the command given to the shell,
   this function will return an array of strings of arguements,
   if any.
*/
/* Arguements:
   -(char *): The command to be parsed.
   Returns: (char **): The arguements
*/
char **arguementify(char *command)
{
  return split_array(command, " ");
}

// Split on Semicolon Function
/* Explanation:
   Given the shell input, this function will parse
   the commands semicolon by semicolon, if any.
*/
/* Arguements:
   -(char *): Shell input
   Returns (struct node*): List of commands
*/
struct node* split_on_semicolon(char *input)
{
  split_list(input, ";");
}

// Split on stdout Function
/* Explanation:
   Given the command, this function splits it into the
   "command" portion and the "file" portion.
*/
/* Arguements:
   -(char *): The command
   Returns (char **): The "command' and "file"
*/
char **split_on_stdout(char *command)
{
  split_array(command, ">");
}

// Split on stdin Function
/* Explanation:
   Given the command, this function splits it into the
   "command" portion and the "file" portion.
*/
/* Arguements:
   -(char *): The command
   Returns (char **): The "command' and "file"
*/
char **split_on_stdin(char *command)
{
  split_array(command, "<");
}

// Split on Append Function
/* Explanation:
   Given the command, this function splits it into the
   "command" portion and the "file" portion.
*/
/* Arguements:
   -(char *): The command
   Returns (char **): The "command' and "file"
*/
char **split_on_append(char *command)
{
  split_array(command, ">>");
}

// Split on Pipe Function
/* Explanation:
   Given the command, this function splits it into
   "command0" and "command1".
*/
/* Arguements:
   -(char *): The command
   Returns (char **): The "command0' and "command1"
*/
struct node* split_on_pipe(char *command)
{
  split_list(command, "|");
}
