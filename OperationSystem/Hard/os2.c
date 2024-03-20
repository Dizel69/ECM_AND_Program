#include <stdio.h> 
#include <signal.h>

int count = 0;
void signalhandler(int _signal) {
  printf("\n recieved SIGINT %d times\n", ++count); 
  if (count == 5)
     signal(SIGINT, SIG_DFL);
  else
     signal(SIGINT, signalhandler);
}
int main(void) {
  signal(SIGINT, signalhandler);
  while (1);
  return 0;
}

