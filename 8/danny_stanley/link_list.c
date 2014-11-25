#include "link_list.h"

char * get_arg(node * current_node){
  char * path = (char*)(malloc(sizeof(char) * 1024));
  printf("test\n");
  printf("%s\n",current_node->arg);
  path = current_node->arg;//segfaulting
  printf("arg: %s\n",path);
  return path;
}

node * get_next(node * current_node){
  node * next_node = current_node->next;
  return next_node;
}

node * get_prev(node * current_node){
  node * previous_node = current_node->prev;
  return previous_node;
}

node * insert_node(node * current_node, char * input){
  node * temp;
  temp = (node *)(malloc(sizeof(node)));
  temp->prev = current_node;
  temp->arg = input;
  return temp;
}
