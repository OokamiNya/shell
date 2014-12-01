#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>

#include "../include/prompt.h"

// Get Username Function
/* Explanation:
   Obtains the username running the shell.
*/
/* Arguements: None
   Returns (char *): The username of the process owner
*/
char *get_username()
{
  struct passwd *entry = getpwuid( geteuid() );

  char *username = (char *) malloc( 33 );
  strcpy(username, entry->pw_name);

  return username;
}

// Get Hostname Function
/* Explanation:
   Obtains the hostname of the machine running the shell.
*/
/* Arguements: None
   Returns (char *): The Hostname of the Machine
*/
char *get_hostname()
{
  char *hostname = (char *) malloc( 65 );

  gethostname(hostname, 64);

  return hostname;
}

// Get Current Working Directory Function
/* Explanation:
   Outputs the formatted current working directory.
   If path contains the home directory, it is replaced with
   "~/".
*/
/* Arguements: None
   Returns (char *): The Current Working Directory (Formated)
*/
char *get_cwd()
{
  struct passwd *entry = getpwuid( geteuid() );

  int size = 1;
  char *cwd = (char *) malloc(size * sizeof(char));

  while (! getcwd(cwd, size) )
  {
    size++;
    cwd = (char *) realloc( cwd, size * sizeof(char) );
  }

  char *check_home_dir = strstr(cwd, entry->pw_dir);
  char *end_part;

  if (! check_home_dir)
    end_part = 0;
  else
    end_part = check_home_dir + strlen(entry->pw_dir);

  char *formatted_cwd = 0;

  if (! end_part)
    formatted_cwd = cwd;
  else
  {
    formatted_cwd = malloc( size + strlen(end_part) + 1 );
    strcpy(formatted_cwd, "~");
    strcat(formatted_cwd, end_part);
  }

  return formatted_cwd;
}


// Print Prompt Function
/* Explanation:
   Prints the prompt for the shell.
   Format:
   <username>@<hostname>:<cwd>$
*/
/* Arguements: None
   Returns (void)
*/
void print_prompt()
{
  char *hostname = get_hostname();
  char *username = get_username();
  char *cwd = get_cwd();

  printf("%s@%s:%s$ ", username, hostname, cwd);

  free(hostname);
  free(username);
  free(cwd);
}
