#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
volatile int count = 0;
volatile int global = 0;
int try(volatile int *mutex){
  return __sync_val_compare_and_swap (mutex , 0 , 1);
}
void lock(volatile int *mutex){
  while(try(mutex)!=0); // spin
}
void unlock(volatile int *mutex){
  *mutex = 0 ;
}
typedef struct args{
  int inc;
  int id;
  volatile int* mutex;
}args ;
void *increment(void *arg){
  int inc = ((args *) arg)->inc;
  int id = (( args *) arg)->id;
  printf("id : %d, inc : %d,%d \n",id,inc,&((args*)arg)->mutex);
  volatile int *mutex = &((args*)arg)->mutex;
  for (int i = 0; i<inc; i++){
    lock(mutex);
    count++;
    unlock(mutex);
  }
}
int main (int argc, char* argv[]){
  if ( argc != 2){
    printf("usage peterson <inc>\n" );
    exit(0);
  }
  int inc = atoi(argv[1]);
  pthread_t one_p, two_p;
  args one_args, two_args;
  one_args.inc = inc;
  two_args.inc = inc;
  one_args.id = 0;
  two_args.id = 1;
  int mut = 0;
  one_args.mutex = mut;
  two_args.mutex = mut;
  pthread_create(&one_p, NULL, increment, &one_args);
  pthread_create(&two_p, NULL, increment , &two_args);
  (void)pthread_join(one_p, NULL);
  (void)pthread_join(two_p, NULL);
  printf ("result is %d \n", count);
  return 0;
}

