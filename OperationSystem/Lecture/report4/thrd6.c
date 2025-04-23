#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h> 
enum { N = 1024 * 1024 };
struct task_data{
  double *data; int n; double s; };
void *mythread(void *vptr){
  struct task_data *pd= (struct task_data*) vptr; 
  int i; double s = 0.0;
  for (i = 0; i < pd->n; ++i) s += pd->data[i];
  pd->s = s;
  return pd;
}
int main(void) {
  double *data = calloc(N, sizeof(data[0]));
  int i;
  pthread_t th1, th2;
  struct task_data t1, t2;
  srand(time(0));
  for (i=0; i<N; ++i) data[i]=rand()/(RAND_MAX+1.0); 
  t1.data = data; t1.n = N / 2;
  t2.data = data + N / 2; t2.n = N-N/2;
  pthread_create(&th1, NULL, mythread, &t1); 
  pthread_create(&th2, NULL, mythread, &t2);
  (void)pthread_join(th1, NULL);
  (void)pthread_join(th2, NULL);
  printf("%.10g\n", (t1.s+t2.s) / N); double s = 0.0;
  for (int i = 0; i < N; ++i) s += data[i];
  printf("%.10g\n", s / N); 
  return 0; 
}
