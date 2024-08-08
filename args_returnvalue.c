#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void* myTurn (void * arg)
{
 int *ptr = (int*)malloc(sizeof(int));
 *ptr = 10;
 for (int i=1; i <= 8; i++)
 {
  sleep(1);
  (*ptr)++;
  printf("My Turn! %d %d\n", i, *ptr);
 }
 return ptr;
}

void* yourTurn (void * arg)
{
 int *ptrs = arg;
 for(int i = 1; i <= 5; i++)
 {
  sleep(1);
  (*ptrs)++;
  printf("Your Turn! %d %d\n", i, *ptrs);
 }
 return NULL;
}

int main()
{
 pthread_t myThread;
 pthread_t yourThread;
 int x = 5;
 int *result;
 pthread_create(&myThread, NULL, myTurn, NULL);
 pthread_create(&yourThread, NULL, yourTurn, &x); //passing arguments in to thread
 pthread_join(myThread, (void *) &result); //return value from thread 
 printf("\n*result = %d\n", *result);
 printf("x = %d\n", x);
 return 0;
}
