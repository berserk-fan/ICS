#include "../csapp.h"

void handler(int signal){
}

unsigned int wakeup(unsigned int secs){
  unsigned int sleep_sec;

  sleep_sec = sleep(secs);
  printf("Woke up at %d of %d secs.\n", secs-sleep_sec, secs);
  return sleep_sec;
}

int main(int argc, char *argv[]){
  if(argc != 2){
    printf("Program takes one argument example:'./7 10'\n");
    return 0;
  }
  int sec = atoi(argv[1]);
  if(!(signal(SIGINT, handler) != SIG_ERR)) {
    printf("some error in signal");
    exit(0);
  }
  wakeup(sec);
  return 0;
}
