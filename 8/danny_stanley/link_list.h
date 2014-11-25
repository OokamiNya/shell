#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct n{
  char* arg;
  struct n* prev;
} node;

node* get_prev(node*);
char* get_arg(node*);
node* insert_node(node*,char*);
