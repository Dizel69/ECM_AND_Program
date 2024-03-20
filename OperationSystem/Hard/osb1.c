#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <signal.h>
int main() {
  pid_t pid = fork();
  if (pid = 0){ //child
      printf("child %d paused (child message)\n", getpid()); 
      pause();
      printf("unreachable\n");
      return 1;
  }
// parent
  sleep(1); // wait for child to print message (dumb way to do it though) 
  if (kill(pid, SIGKILL) == 0) {
      printf("child %d is dead (parent message) \n", pid);
  }
  return 0;
}
