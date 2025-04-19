#include <signal.h> 
#include <stdio.h>
#include <stdlib.h>

void handler(int _signal) {
  printf("caught signal %d\n", _signal); 
  exit(1);
}
int divide_by_zero() {
  int x = 0;
  return 1 % x;
}
int main(void) {
  struct sigaction sa; 
  sa.sa_handler = handler; 
  sigemptyset(&sa.sa_mask);
  sigaction (SIGFPE, &sa, NULL);
  int result = divide_by_zero();
  printf("This won't execute; result: %d\n", result); return 0;
}
