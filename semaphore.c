#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define THREAD_NUM 4

sem_t semaphore;

void* routine(void* args)
{
	sem_wait(&semaphore);
	sleep(1);
	printf("Hello from thread %d\n", *(int*)args);
	free(args);
	sem_post(&semaphore);
}

int main (int argc, char* argv[])
{
	pthread_t th[THREAD_NUM];
	sem_init(&semaphore, 0, 2);
	
	int i;
	for (i = 0; i < THREAD_NUM; i++)
	{
		int* a = (int*) malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &routine, a) != 0)
		{	perror("Failed to create thread\n");	}
	}
	
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{	perror("Failed to join thread\n");	}
	}
	sem_destroy(&semaphore);
	return 0;
}
