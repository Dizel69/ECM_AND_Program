#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
void handler1(int _signal) {
  int olderrno = errno;
  // original book example used safe I/O operations 
  // (i hope everything is gonna be fine with standart ones) 
  if (waitpid(-1, NULL, 0) < 0) {
      printf("waitpid error. aborting...\n"); 
      abort();
  }
  puts("handler reaped some child");
  sleep(1);
  errno = olderrno;
}
int main() {
  if (signal (SIGCHLD, handler1)==SIG_ERR) {
      printf("signal error. exiting...\n");
      return 1;
  }
  for (int i = 0; i < 3; ++i) {
       if (fork() == 0) { // child
           printf("Hello from child %d\n", getpid()); 
           return 0;
       }
  }
  char buf[80];
  // i suppose STDIN_FILENO is 0
  if (read (STDIN_FILENO, buf, sizeof(buf)) < 0) { 
      printf("read error. exiting...\n"); 
      return 1;
  }
  printf("got some input: '%s'\n", buf);
  while (1);
  return 0;
}
