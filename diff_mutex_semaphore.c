#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define THREAD_NUM 4

int fuel = 50;
sem_t semFuel;
pthread_mutex_t mutexFuel;

void* routine1 (void* args)
{
	while (1)
	{
		// pthread_mutex_lock(&mutexFuel);  // locking the mutex in one thread
		sem_wait(&semFuel);
		fuel += 50;
		printf("Current value is %d\n", fuel);
	}
}

void* routine2 (void* args)
{
	while (1)
	{
		// pthread_mutex_unlock(&mutexFuel); // when you try to unlock mutex from another thread this is incorrect
					// because mutexes are locked and unlocked by the same thread only
					// hence using mutex program will terminate after some time with error like below
			// Fatal glibc error: pthread_mutex_lock.c:94 (___pthread_mutex_lock): assertion failed: mutex->__data.__owner == 0
			// Aborted (core dumped)

		sem_post(&semFuel);		     // But with the semaphore there is no issue with locking and unlocking using
						     // different different threads
						     // No error, program will run smothly like intended
		usleep(50000);
	}
}

int main (int argc, char* argv[])
{
	pthread_t th[THREAD_NUM];
	sem_init(&semFuel, 0, 1);
	pthread_mutex_init(&mutexFuel, NULL);
	
	int i;
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (i % 2 == 0)
		{
			if (pthread_create(&th[i], NULL, &routine1, NULL) != 0)
			{	perror("Failed to create thread\n");	}
		}
		else
		{
			if (pthread_create(&th[i], NULL, &routine2, NULL) != 0)
			{	perror("Failed to create thread\n");	}
		}
	}
	
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{	perror("Failed to join thread\n");	}
	}
	
	sem_destroy(&semFuel);
	pthread_mutex_destroy(&mutexFuel);
	return 0;
}
