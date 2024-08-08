#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 1

int *fuel;
sem_t semFuel;

void* routine(void* args)
{

	*fuel += 50;
	printf("Current value of fuel: %d\n", *fuel);
	// sleep(1);
	sem_post(&semFuel);
}

int main(int argc, char* argv[])
{
	pthread_t th[THREAD_NUM];
	fuel = (int*)malloc(sizeof(int));
	*fuel = 50;

	sem_init(&semFuel, 0, 0); // binary semaphore can only have value 0 or 1
	
	int i;
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
		{	perror("Failed to create thread\n");	}
	}
	
	sem_wait(&semFuel);
	printf("Deallocating fuel\n");
	free(fuel);

	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{	perror("Failed to join thread\n");	}
	}

	sem_destroy(&semFuel);
	return 0;
}
