#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <linux/futex.h>
#include <syscall.h>
#define MAX 100
typedef struct cell{
  int val;
  struct cell *next;
}cell;
cell sentinel = {MAX, NULL};
cell dummy = {-1, &sentinel};
cell *global = &dummy;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void toggle(cell *lst, int r){
  cell *prev = NULL;
  cell *this = lst;
  cell *removed = NULL;
  pthread_mutex_lock(&mutex);
  while(this->val<r){
    prev = this;
    this = this->next;
  }
  if(this->val==r){
    prev->next=this->next;
    removed=this;
  }else{
    cell *new = malloc(sizeof(cell));
    new->val=r;
    new->next=this;
    prev->next=new;
  }
  pthread_mutex_unlock(&mutex);
  if(removed !=NULL) free(removed);
    return;
}
int futex_wait(volatile int *futexp){
  return syscall(SYS_futex, futexp, FUTEX_WAIT, 1, NULL, NULL, 0);
}
void futex_wake(volatile int *futexp){
  syscall(SYS_futex, futexp, FUTEX_WAKE, 1, NULL, NULL, 0);
}
volatile int count = 0;
int try(volatile int *mutex){
  return __sync_val_compare_and_swap (mutex , 0 , 1);
}
int lock(volatile int *lock){
  int susp = 0;
  while(try(lock)!=0){
    susp++;
    count++;
    futex_wait(lock);
  }
  return susp;
}
void unlock(volatile int *lock){
  *lock = 0 ;
  futex_wait(lock);
}
typedef struct args{
  int inc;
  int id;
  volatile int* mutex;
  cell *list;
}args ;
void *bench(void *arg){
  int inc = ((args *)arg)->inc;
  int id = ((args *)arg) ->id;
  cell *lstp = ((args *)arg)->list;
  for (int i=0; i<inc;i++){
    int r = rand()%MAX;
    toggle(lstp, r);
  }
}
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
    printf("usage: <list><thread>\n" );
  }
  int n = atoi(argv[1]);
  int inc = (atoi(argv[1])/n);
  printf("%d threads doing %d operation each \n",n, inc);
  pthread_mutex_init(&mutex, NULL);
  args *thra = malloc(n * sizeof(args));
  for(int i=0; i<n; i++){
    thra[i].inc = inc;
    thra[i].id = i;
    thra[i].list = global;
  }
  pthread_t *thrt = malloc(n * sizeof(pthread_t));
  for(int i=0; i<n; i++){
    pthread_create(&thrt[i], NULL, bench, &thra[i]);
  }
  for(int i=0; i<n; i++){
    pthread_join(thrt[i], NULL);
  }
  printf("done\n");
  return 0;
}

