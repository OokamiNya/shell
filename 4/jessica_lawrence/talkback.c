#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  char* s[256];
  fgets(s, 256, stdin);
  printf("#####\n%s#####\n",s);
  return 0;
}
