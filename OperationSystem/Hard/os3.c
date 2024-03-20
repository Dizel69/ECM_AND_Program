#include <stdio.h>
#include <signal.h> 
#include <unistd.h>
volatile int count = 0;

void signalhandler(int _signal) {
  printf("recieved signal %d\n", _signal);
  ++count;
}

int main(void) {
  int pid = getpid();
  printf("now try to kiil me (pid: %d)\n", pid);
  struct sigaction sa;
  sa.sa_handler = signalhandler;
  sigemptyset(&sa.sa_mask); sigaction (SIGINT, &sa, NULL);
  while (count != 4) {
       kill(pid, SIGINT);
  }
  printf("recieved 4 SIGINTs, exiting\n");
  return 0;
}
