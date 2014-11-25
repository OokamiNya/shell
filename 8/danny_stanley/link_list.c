#include "link_list.h"

char * get_arg(node * current_node){
  char * arg;
  arg = current_node->arg;
  return arg;
}

node * get_next(node * current_node){
  next_node = current_node->next;
  return next_node;
}

node * get_prev(node * current_node){
  previous_node = current_node->prev;
  return previous_node;
}

node * insert_node(node * current_node, char * input){
  node * temp;
  temp = (node *)(malloc(sizeof(node)));
  temp->prev = current_node;
  temp->arg = input;
  return temp;
}
