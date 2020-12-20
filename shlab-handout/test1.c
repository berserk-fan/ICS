#include "stdio.h"

#define BUILT_IN_COMMAND_MAX_LENGTH 4
char built_in[][BUILT_IN_COMMAND_MAX_LENGTH] = {"jobs", "bg", "fg", "quit"};

int main(){
  printf("%d", (int)sizeof(built_in));
  
  return 0;
}
