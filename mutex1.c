#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>
#include <unistd.h>

int count = 0;
pthread_mutex_t mutex;

void* routine(void* args)
{
 
 for (int i = 0; i < 10000; i++)
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
 pthread_t t1, t2;
 if (pthread_create(&t1, NULL, routine, NULL) != 0)
 { printf("Error while creating t1"); }
 
 if (pthread_create(&t2, NULL, routine, NULL) != 0)
 { printf("Error while creating t2"); }
 
 if (pthread_join(t1, NULL) != 0)
 { printf("Error while joining t1"); }

 if (pthread_join(t2, NULL) != 0)
 { printf("Error while joining t2"); }
 
 pthread_mutex_destroy(&mutex);
 printf("Value of count: %d\n", count);
 return 0;
}
