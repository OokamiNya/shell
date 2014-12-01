#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/execute.h"
#include "../include/cd.h"
#include "../include/list.h"
#include "../include/parse.h"
#include "../include/error.h"

// Run Commands Function
/* Explanation:
   Given a linked list of commands, this function
   will run them sequentially.
*/
/* Arguements:
   -(struct node*): The List of Commands
   Returns (void)
*/
void run_commands(struct node* commands)
{
  while (commands)
  {
    char **array = 0;
    struct node* list = 0;

    // Redirect append
    if ( strstr( commands->data, ">>" ) )
    {
      array = split_on_append(commands->data);
      redirect_append(array);
    }
    // Redirect stdout
    else if ( strstr( commands->data, ">" ) )
    {
      array = split_on_stdout(commands->data);
      redirect_stdout(array);
    }
    // Redirect stdin
    else if ( strstr( commands->data, "<" ) )
    {
      array = split_on_stdin(commands->data);
      redirect_stdin(array);
    }
    // Pipe
    else if ( strstr( commands->data, "|" ) )
    {
      list = split_on_pipe(commands->data);
      redirect_pipe(list);
    }
    // Normal Execution
    else
    {
      array = arguementify(commands->data);
      execute_command(array);
    }

    // Cleanup
    free_list(list);
    free_string_array(array);

    // Continue
    commands = commands->next;
  }
}

// Execute Command Function
/* Explnation:
   Given (char **) "arguements", the parent (shell) will fork
   off a child and the child will execute the command w/ arguements.
   An error will be displayed if an error occurs.
*/
/* Arguements:
   -(char **): Array of Arguements
   Returns (void)
*/
static void execute_command(char **arguements)
{
  if ( (! arguements) ||
       (! **arguements))
    return;

  // Special Cases
  if (! strcmp(arguements[0], "exit") )
    exit(0);
  else if (! strcmp(arguements[0], "cd") )
    cd( arguements[1] );

  // Else
  else
  {
    int child = fork();

    // Parent
    if (child)
    {
      int status;
      wait(&status);
    }

    // Child
    else
    {
      int status = execvp( arguements[0], arguements );

      if (status == -1)
	printf("normandy: command not found\n");

      exit(0);
    }
  }
}

// Redirect Stdout Function
/* Explanation:
   This function redirects stdout to a flie (truncate).
   array[0] = command
   array[1] = file
*/
/* Arguements:
   -(char **): Array of elements
   Returns (void)
*/
static void redirect_stdout(char **array)
{
  if ( (! array) ||
       (! array[0]) ||
       (! array[1]))
    return;

  char **arguements = arguementify( array[0] );
  int fd = open( array[1], O_WRONLY | O_CREAT | O_EXCL, 0644);
  if (fd == -1)
    fd = open( array[1], O_WRONLY | O_TRUNC );

  // If I can't really open the file...
  if (fd == -1)
  {
    print_error("open", errno);
    return;
  }

  // Otherwise, keep going
  int new_stdout = dup(STDOUT_FILENO);
  dup2(fd, STDOUT_FILENO);

  execute_command( arguements );

  // Cleanup
  dup2(new_stdout, STDOUT_FILENO);
  close(fd);
  close(new_stdout);
  free(arguements);
}

// Redirect stdin Function
/* Explanation:
   This function redirects stdin to a flie.
   array[0] = command
   array[1] = file
*/
/* Arguements:
   -(char **): Array of elements
   Returns (void)
*/
static void redirect_stdin(char **array)
{
  if ( (! array) ||
       (! array[0]) ||
       (! array[1]))
    return;

  char **arguements = arguementify( array[0] );
  int fd = open( array[1], O_WRONLY | O_CREAT | O_EXCL, 0644);
  if (fd == -1)
    fd = open( array[1], O_WRONLY | O_TRUNC );

  // If I can't really open the file...
  if (fd == -1)
  {
    print_error("open", errno);
    return;
  }

  // Otherwise, keep going
  int new_stdin = dup(STDIN_FILENO);
  dup2(fd, STDIN_FILENO);

  execute_command( arguements );

  // Cleanup
  dup2(new_stdin, STDIN_FILENO);
  close(fd);
  close(new_stdin);
  free_string_array(arguements);
}

// Redirect Append Function
/* Explanation:
   This function redirects stdout to a flie (append).
   array[0] = command
   array[1] = file
*/
/* Arguements:
   -(char **): Array of elements
   Returns (void)
*/
static void redirect_append(char **array)
{
  if ( (! array) ||
       (! array[0]) ||
       (! array[1]))
    return;

  char **arguements = arguementify( array[0] );
  int fd = open( array[1], O_WRONLY | O_CREAT | O_EXCL, 0644);
  if (fd == -1)
    fd = open( array[1], O_WRONLY | O_APPEND );

  // If I can't really open the file...
  if (fd == -1)
  {
    print_error("open", errno);
    return;
  }

  // Otherwise, keep going
  int new_stdout = dup(STDOUT_FILENO);
  dup2(fd, STDOUT_FILENO);

  execute_command( arguements );

  // Cleanup
  dup2(new_stdout, STDOUT_FILENO);
  close(fd);
  close(new_stdout);
  free_string_array(arguements);
}

// Redirect Pipe Function
/* Explanation:
   This function redirects cmmand0's stdout
   to command1's stdin.
*/
/* Arguement:
   -(struct node*): List of Commands to pipe
   Returns (void)
*/
static void redirect_pipe(struct node* list)
{
  if ( (! list) ||
       (! list->data) ||
       (! list->next) ||
       (! list->next->data))
    return;

  char **cmd0_args = arguementify( list->data );
  char **cmd1_args = arguementify( list->next->data );
  char temp_file[] = "/tmp/normandy_XXXXXX";

  int tmp_fd = mkstemp(temp_file);

  int cmd0_stdout = dup(STDOUT_FILENO);
  dup2(tmp_fd, STDOUT_FILENO);

  execute_command( cmd0_args );

  // Reset stdout
  dup2(cmd0_stdout, STDOUT_FILENO);

  // Change stdin (cmd1)
  int cmd1_stdin = dup(STDIN_FILENO);
  dup2(tmp_fd, STDIN_FILENO);

  execute_command( cmd1_args );

  // Cleanup
  dup2(cmd1_stdin, STDIN_FILENO);
  close(tmp_fd);
  close(cmd0_stdout);
  close(cmd1_stdin);
  free_string_array(cmd0_args);
  free_string_array(cmd1_args);
}
