#include <stdio.h>
int main(){
  int tmp = 1;
  char *pt= (char *) &tmp;
  if(*pt == 1){
    printf("Your OS is little endian.\n");
  }else if(*pt == 0){
    printf("Your OS is big endian.\n");
  }else{
    printf("Your OS has unknown endianness.\n");
  }
  return 0;
}
