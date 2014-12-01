#include "heads.h"

char * strip(char * s){
  while (*s == ' ' || *s == '\t' || *s == '\n'){
    s++;
  }
  char length = 0;
  while (*(s+strlen(s)-1-length) == ' ' || *(s+strlen(s)-1-length) == '\t' || *(s+strlen(s)-1-length) == '\n'){
    length++;
  }
  char * answer = malloc(strlen(s)-length); //THIS CANNOT GO WELL
  strncpy(answer,s,strlen(s)-length);
  return answer;
}

