#include <stdlib.h> 
#include <signal.h> 
#include <stdio.h> 
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

typedef void (*handler_t) (int);
// no typedef declaration is a mess...
// void (*Signal (int _signal, void (*handler) (int)))(int) { 
handler_t Signal(int _signal, handler_t handler) { 
  struct sigaction action, oldaction;
  action.sa_handler=handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;
  if (sigaction(_signal, &action, &oldaction) < 0) { 
      printf("Signal error. aboting...\n"); 
      abort();
  }
  return oldaction.sa_handler;
}

volatile sig_atomic_t pid;
void sigchld_handler(int _signal) {
  int olderrno = errno;
  pid = waitpid(-1, NULL, 0);
  errno = olderrno;
}
void sigint_handler(int signal) {}

int main() {
  sigset_t mask, prev;
  Signal (SIGCHLD, sigchld_handler); 
  Signal (SIGINT, sigint_handler);
  sigemptyset (&mask);
  sigaddset (&mask, SIGCHLD);
  sigprocmask (SIG_BLOCK, &mask, &prev);
  if (fork() == 0) { // child
      return 0;
  }
  pid = 0;
  while (!pid){
       sigsuspend(&prev);
  }
  sigrocmask(SIG_SETMASK, &prev, NULL);
  printf("done\n");
  return 0;
}
