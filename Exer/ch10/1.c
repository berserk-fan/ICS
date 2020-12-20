#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

void printerror(const char *message, int errno){
  fprintf(stderr,"%s: %s\n", message, strerror(errno)); 
}

int Open(const char *file,int flags, mode_t mode){
  int errno;
  if((errno = open(file,flags,mode)) < 0)
    printerror("Open error", errno);
  return errno;
}

int Close(int descriptor){
  int errno;
  if((errno = close(descriptor)) < 0)
    printerror("Close error", errno);
  return errno;
}
int main() {
  int fd1, fd2;

  fd1 = Open("foo.txt", O_RDONLY, 0);
  Close(fd1);
  fd2 = Open("baz.txt", O_RDONLY, 0);
  printf("fd2 = %d\n", fd2);
  exit(0);
}
