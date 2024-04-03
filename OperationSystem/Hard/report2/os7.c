
#include <unistd.h> 
#include <signal.h> 
#include <fcntl.h>
#include <stdlib.h>

void signalhandler(int _signal) {
  printf("\ncaught signal %d, waiting for a name... .\n", _signal);    
  signal (SIGALRM, signalhandler);
}
int main(void) {
  char name[80];
  signal (SIGALRM, signalhandler);
  int pid=fork();
  if (pid) { // parent section
     while (1) {
          sleep (5);
          kill(pid, SIGALRM);
     }
  }
  else { // child section
      printf("Input a name: ");
      while (1) {
           if (fgets(name, sizeof (name), stdin)) 
               break;
           }
           printf("Hi, %s", name); 
           kill(getppid(), SIGINT);
      }
  return 0;
}
