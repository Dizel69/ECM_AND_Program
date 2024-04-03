#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
int max;
volatile int counter = 0;
void *mythread(void *arg){
  char *letter = arg;
  int i;
  printf("%s: begin\n", letter);
  for (i = 0; i < max; i++)
      counter = counter + 1;
  printf("%s: done\n", letter);
  return NULL;
}
int main(int argc, char *argv[]){
  max = atoi(argv[1]);
  pthread_t p1, p2;
  printf("main: begin[couner = %d]\n", counter);
  pthread_create(&p1, NULL, mythread, "A");
  pthread_create(&p2, NULL, mythread, "B");
  (void)pthread_join(p1, NULL);
  (void)pthread_join(p2, NULL);
  printf("main: done\n[counter: %d]\n[should: %d]\n", counter, max*2);
  return 0;
}
