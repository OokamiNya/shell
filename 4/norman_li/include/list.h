#ifndef LIST_H
#define LIST_H

// Node STructure
struct node
{
  char *data;
  struct node* next;
};

// Functions
struct node* insert_node(struct node*, char *);      // Insert Function
void free_list(struct node*);                        // Free List Function
char **return_string_array(struct node*);            // Return Character Array Function
void free_string_array(char **);                     // Free String Array

#endif
