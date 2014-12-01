#include"heads.h"

int main(){
  printf("I NEED INPUT PLZ\n");
  char input[256];
  fgets(input,sizeof(input),stdin);
  input[strlen(input)-1]=0;
  printf("I WORK: %s\n",input);
  return 0;
}
