#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* myTurn (void * arg)
{
 for (int i=1; i <= 8; i++)
 {
  sleep(1);
  printf("My Turn! %d\n", i);
 }
 return NULL;
}

void yourTurn ()
{
 for(int i = 1; i <= 5; i++)
 {
  sleep(1);
  printf("Your Turn! %d\n", i);
 }
}

int main()
{
 pthread_t myThread;
 pthread_create(&myThread, NULL, myTurn, NULL);
 yourTurn();
 pthread_join(myThread, NULL);
 //yourTurn();
 return 0;
}
