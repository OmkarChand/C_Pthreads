#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 8

int buffer[10];
int count = 0;

pthread_mutex_t mutexBuffer;
sem_t semFull, semEmpty;

void* producer(void* args)
{
	// Produce
	while (1) // producer contineously produces data
	{
		int x = rand() % 100;
		// sleep(1);		
		// add to the buffers
		sem_wait(&semEmpty);
		pthread_mutex_lock(&mutexBuffer);
		buffer[count] = x;
		count++;
		pthread_mutex_unlock(&mutexBuffer);
		sem_post(&semFull);
	}
}

void* consumer(void* args)
{
	while (1)
	{
		int y = -1;
	
		// remove from the buffer
		sem_wait(&semFull);
		pthread_mutex_lock(&mutexBuffer);
		y = buffer[count - 1];
		count--;
		pthread_mutex_unlock(&mutexBuffer);
		sem_post(&semEmpty);
		
		// consume
		printf("Got %d\n", y);
		sleep(1); 
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_t th[THREAD_NUM];
	pthread_mutex_init(&mutexBuffer, NULL);

	sem_init(&semEmpty, 0, 10); // initial value of semaphore is 10 because buffer size is 10
	sem_init(&semFull, 0, 0); // initial value is 0 because initially buffer is empty
	
	int i;
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (i == 0) // only one producers and other are consumers
		{
			if (pthread_create(&th[i], NULL, &producer, NULL) != 0)
			{	perror("Failed to cread producer thread\n");	}
		}
		else
		{
			if (pthread_create(&th[i], NULL, &consumer, NULL) != 0)
			{	perror("Failed to cread consumer thread\n");	}
		}
	}
	
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{	perror("Failed to join thread");	}
	}
	
	pthread_mutex_destroy(&mutexBuffer);
	sem_destroy(&semEmpty);
	sem_destroy(&semFull);	
}
