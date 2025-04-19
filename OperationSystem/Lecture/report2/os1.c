#include <stdio.h> 
#include <signal.h> 
#include <stdlib.h>
void signalhandler(int signal) {
  printf("caught signal %d, exiting...\n", signal); 
  exit(1);
}

int main(void) {
  signal(SIGINT, signalhandler);
  while (1);
  return 0;
}
