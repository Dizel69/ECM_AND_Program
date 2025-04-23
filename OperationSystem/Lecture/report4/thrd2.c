#include <assert.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h>
typedef struct { 
int a;
int b; 
}myarg_t;
void *mythread(void *arg) {
  myarg_t *args = (myarg_t *) arg;
  printf("%d %d\n", args->a, args->b);
  myarg_t *t = (myarg_t*)malloc(sizeof(myarg_t));
  t->a=1;
  t->b=2;
  return t;
}
int main(int argc, char *argv[]) {
  pthread_t p;
  myarg_t args = { 10, 20 };
  pthread_create(&p, NULL, mythread, &args); 
  myarg_t *ret;
  (void) pthread_join(p, (void**)&ret); 
  printf("returned %d %d\n", ret->a, ret->b); 
  free(ret);
  printf("done\n");
  return 0;
}
