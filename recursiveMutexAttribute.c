
// Recursive mutex attribute is used to use the same mutext recursively in the same thread, but make sure that you have unlocked the same time as you lock the same mutex in the same thread otherwise it will cause deadlock

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define PTHREAD_NUM 8

pthread_mutex_t mutexFuel;
int fuel = 50;

void* routine(void* arg)
{
	pthread_mutex_lock(&mutexFuel);
	pthread_mutex_lock(&mutexFuel);
	pthread_mutex_lock(&mutexFuel);
	fuel += 50;
	printf("Fuel incremented to: %d\n", fuel);
	pthread_mutex_unlock(&mutexFuel);
	pthread_mutex_unlock(&mutexFuel);
	pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char* argv[])
{
	pthread_t th[PTHREAD_NUM];
	pthread_mutexattr_t recursiveMutexAttr;
	pthread_mutexattr_init(&recursiveMutexAttr);
	pthread_mutexattr_settype(&recursiveMutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutexFuel, &recursiveMutexAttr);
	int i;
	for (i = 0; i < PTHREAD_NUM; i++)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
		{	perror("Failed to create thread");	}
	}
	for (i = 0; i < PTHREAD_NUM; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{	perror("Failed to join thread");	}
	}
	printf("Fuel: %d\n", fuel);
	pthread_mutexattr_destroy(&recursiveMutexAttr);
	pthread_mutex_destroy(&mutexFuel);
	return 0;
}
