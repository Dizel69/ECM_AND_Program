#include <signal.h> 
#include <stdio.h> 
#include <unistd.h>
void handler(int _signal) {
  printf("\nwaited too long: caught signal %d\n", _signal);
  alarm(5);
  signal (SIGALRM, handler);
}
int main(void) {
  char name[80];
  signal (SIGALRM, handler); alarm(5);
  printf("Input a name: ");
  while (1) {
       if (fgets(name, sizeof(name), stdin)) 
           break;
  }
  printf("Hi, %s", name);
  return 0;
}
