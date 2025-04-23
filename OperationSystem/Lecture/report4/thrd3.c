#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h> 
void *f(void *arg) {
  printf("%s\n", (char *) arg); 
  return NULL;
}
int main(int argc, char *argv[]) {
  pthread_t p1, p2;
  printf("main: begin\n");
  pthread_create(&p1, NULL, f, "A"); 
  pthread_create(&p2, NULL, f, "B"); 
  (void)pthread_join(p1, NULL);
  (void)pthread_join(p2, NULL);
  printf("main: end\n");
  return 0;
}
