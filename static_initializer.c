#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int x = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* routine(void* arg)
{
	int a = *(int*)arg;
	sleep(1);
	pthread_mutex_lock(&mutex);
	x++;
	pthread_mutex_unlock(&mutex);
	printf("Finished execution... ID: %d\n", a);
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t th[4];
	int i;
	for (i = 0; i < 4; i++)
	{
		int* a = (int*) malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &routine, a) != 0)
		{	perror("Failed to create thread\n");	}
	}
	for (i = 0; i < 4; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{	perror("Failed to join thread\n");	}
	}
	printf("X: %d\n", x);
	return 0;
}
