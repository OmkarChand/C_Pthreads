#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_barrier_t barrier;

void* routine(void* arg)
{
	while (1)
	{
		printf("Waiting at the barrier...\n");
		sleep(1);
		pthread_barrier_wait(&barrier);
		printf("We passed the barrier.\n");
		sleep(1);
	}
	return NULL;
}

int main(int argc, char* argz[])
{
	int n = 10;
	pthread_t th[n];
	
	pthread_barrier_init(&barrier, NULL, 7);
	int i;
	for (i = 0; i < n; i++)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
		{ perror("Failed to create thread"); }
	}

	for (i = 0; i < n; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{ perror("Failed to join thread"); }
	}
	pthread_barrier_destroy(&barrier);
	return 0;
}
