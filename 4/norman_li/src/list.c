#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"

// Insert Node Function
/* Explanation:
   Given a (char *) string, this function inserts
   a node with the string at the end of the linked
   list.
*/
/* Arguements:
   -(struct node*): The head node of the list
   -(char *): The string to be added
   Returns (void)
*/
struct node* insert_node(struct node* head, char *data)
{
  struct node* temp = head;
  struct node* new_node = (struct node*) malloc( sizeof( struct node ) );

  new_node->data = (char *) malloc( strlen(data) + 1 );
  strcpy( new_node->data, data );
  new_node->next = 0;

  if (! head)
    return new_node;

  while (head->next)
    head = head->next;

  head->next = new_node;

  return temp;
}

// Free List Function
/* Explanation:
   Given the head node, this function will free the entire list,
   include the malloc'ed "data" strings.
*/
/* Arguements:
   -(struct node*): Head Node of List
   Returns (void)
*/
void free_list(struct node* head)
{
  while (head)
  {
    struct node* next = head->next;

    free( head->data );
    free( head );

    head = next;
  }
}

// Return String Array
/* Explanation:
   Given the head node, this function will return the array of
   character arrays of the linked list
*/
/* Arguements:
   -(struct node*): The Head Node
   Returns (char **): The array of character arrays
*/
char **return_string_array(struct node* head)
{
  char **string_array = 0;
  int size = 0;

  while(head)
  {
    size++;
    string_array = (char **) realloc( string_array, size * sizeof(char *) );

    string_array[size - 1] = (char *) malloc( strlen(head->data) + 1 );
    strcpy( string_array[size - 1], head->data );

    head = head->next;
  }

  if (size)
  {
    size++;
    string_array = (char **) realloc( string_array, size * sizeof(char *) );
    string_array[size - 1] = 0;
  }

  return string_array;
}

// Free String Array
/* Explanation:
   Given a "string array" generated from the function "(char **) return_string_array",
   this function will free all elements of the array, including the malloc'ed strings
   themselves.
*/
/* Arguements:
   -(char **): The array to be freed
   Returns (void)
*/
void free_string_array(char **string_array)
{
  if (! string_array)
    return;

  char **temp = string_array;

  while( *string_array )
  {
    free( *string_array );
    string_array++;
  }

  free(temp);
}
