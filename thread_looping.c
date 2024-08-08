#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>
#include <unistd.h>

int count = 0;
pthread_mutex_t mutex;

void* routine(void* args)
{
 
 for (int i = 0; i < 10000000; i++)
 {
  pthread_mutex_lock(&mutex);
  count++;
  pthread_mutex_unlock(&mutex);
 }
 return NULL;
}

int main()
{
 pthread_mutex_init(&mutex, NULL);
 pthread_t t[4];
 for (int i = 0; i < 4; i++)
 {
  if (pthread_create(&t[i], NULL, routine, NULL) != 0)
  { printf("Error while creating t[%d]", i); }
 }
 for (int j = 0; j < 4; j++)
 {
  if (pthread_join(t[j], NULL) != 0)
  { printf("Error while joining t[%d]", j); }
 }
 pthread_mutex_destroy(&mutex);
 printf("Value of count: %d\n", count);
 return 0;
}
