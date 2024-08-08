#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define THREAD_NUM 4

sem_t sem;

void* routine (void* args)
{
	int semValue;
	int index = *(int*)args;
	sem_wait(&sem);
	sleep(index + 1);
	sem_getvalue(&sem, &semValue);
	printf("(%d) sem value after wait: %d\n", index, semValue);
	sem_post(&sem);
	sem_getvalue(&sem, &semValue);
	printf("(%d) sem value after post: %d\n", index, semValue);	
	free(args);
}

int main(int argc, char* argv[])
{
	pthread_t th[THREAD_NUM];
	sem_init(&sem, 0, 4);
	
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
	
	sem_destroy(&sem);
	return 0;
}
