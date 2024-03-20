#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
void signalhandler(int _signal) {
  printf("signal %d (SIGINT) caught. exiting...\n", _signal); 
  exit(0);
}
int main() {
  if (signal(SIGINT, signalhandler) == SIG_ERR) {
     printf("signal error. exiting...\n");
     exit(1);
  }
  pause();
  return 0;
}
