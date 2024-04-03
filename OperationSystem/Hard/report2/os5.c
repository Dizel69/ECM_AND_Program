#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

const char* filename = "tempfile";

void signalhandler(int _signal) { 
  printf("caught signal %d\n", _signal); 
  unlink(filename);
  exit(0);
}

int main(void) {
  signal(SIGINT, signalhandler);
  creat(filename, 666);
  int seconds = 8;
  for (int i = 0; i < seconds; ++i) {
      printf("progam will end in %d seconds\n", seconds - i); 
      sleep(1);
  }
  unlink(filename);
  return 0;
}

