#include "../csapp.h"

unsigned int wakeup(unsigned int secs){
  unsigned int sleep_sec;

  sleep_sec = sleep(secs);
  printf("Woke up at %d secs.\n", secs-sleep_sec);
  return sleep_sec;
}

int main(){
  wakeup(5);
  return 1;
}
