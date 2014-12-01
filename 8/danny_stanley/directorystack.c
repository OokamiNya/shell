#include "directorystack.h"

char * get_arg(node * current_node){
  char * path;
  path = current_node->arg;
  return path;
}

node * get_prev(node * current_node){
  if(current_node->prev != NULL){
    node * previous_node = current_node->prev;
    free(current_node);
    return previous_node;
  }
  return current_node;
}

node * insert_node(node * current_node, char * input){
  node * temp;
  temp = (node *)(malloc(sizeof(node)));
  temp->prev = current_node;
  temp->arg = input;
# ifdef DEBUG
  printf("PATH Written: %s\n",temp->arg);
# endif
  return temp;
}
