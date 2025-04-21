#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#define MAX 10
static int running;
static ucontext_t cntx_one;
static ucontext_t cntx_two;
static ucontext_t cntx_main;
void yield(){
  printf(" -yield-\n");
  if (running==1){
    running = 2;
    swapcontext(&cntx_one, &cntx_two);
  } else{
    running = 1;
    swapcontext(&cntx_two, &cntx_one);
  }
}
void push(int p, int i){
  if (i<MAX){
    printf("%d%*s push\n", p, i, " ");
    push(p, i+1);
    printf("%d%*s pop\n", p, i, " ");
  } else{
    printf("%d%*s top\n", p , i, " ");
    yield();
  }
}
int main(){
  char stack1[8 * 1024];
  char stack2[8 * 1024];
  getcontext(&cntx_one);
  cntx_one.uc_stack.ss_sp = stack1;
  cntx_one.uc_stack.ss_size = sizeof stack1;
  makecontext(&cntx_one, (void (*) (void)) push, 2, 1, 1);
  getcontext(&cntx_two);
  cntx_two.uc_stack.ss_sp = stack2;
  cntx_two.uc_stack.ss_size = sizeof stack2;
  makecontext(&cntx_two, (void(*) (void)) push, 2, 2, 1);
  running = 1;
  swapcontext(&cntx_main, &cntx_one);
  return 0;
}
