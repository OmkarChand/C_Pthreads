#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 2

int buffer[10];
int count = 0;

pthread_mutex_t mutexBuffer;
// without using semaphore the loop will always generate the data
// and since buffer size is limited hence some data will be skipped

// to avoid this we nee 2 semaphores one to track the buffer
// full and another to track buffer empty

void* producer(void* args)
{
	// Produce
	while (1) // producer contineously produces data
	{
		int x = rand() % 100;
		
		// add to the buffers
		pthread_mutex_lock(&mutexBuffer);
		if (count < 10)	// to handle segmentation fault
		{
			buffer[count] = x;
			count++;
		}
		else // problem here is, many data are skipped due to buffer full and to serve this cpu cycles are wasted
		{
			printf("Skipped %d\n", x); // to check how many data are skipped
		}
		pthread_mutex_unlock(&mutexBuffer);
	}
}

void* consumer(void* args)
{
	while (1)
	{
		int y = -1;
	
		// remove from the buffer
		pthread_mutex_lock(&mutexBuffer);
		if (count > 0)
		{	
			y = buffer[count - 1];
			count--;
		}
		pthread_mutex_unlock(&mutexBuffer);
		
		// consume
		printf("Got %d\n", y); // problem here is, many time "Got -1" will be printed when the buffer will become empty, and to serve this cpu cycles are wasted. need to be controlled
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_t th[THREAD_NUM];
	pthread_mutex_init(&mutexBuffer, NULL);

	int i;
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (i % 2 == 0) // half of the threads are producers and other halfs are consumers
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
}
