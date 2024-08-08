#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h> 
#include <unistd.h> // for sleep()

pthread_mutex_t mutex;

void* routine() {
	// pthread_mutex_lock(&mutex); // if you use lock()
				 // then it will wait untill got the lock
	// but if you use trylock() then it won't wait untill lock will be
	// free instead it will return instantly
	if (pthread_mutex_trylock(&mutex) == 0)
	{
		printf("Got Lock\n");
		sleep(1);
		pthread_mutex_unlock(&mutex);
	}
	else
	{
		printf("Didn't get Lock\n");
	}
}

int main(int argc, char* argv)
{
	pthread_t th[4];
	pthread_mutex_init(&mutex, NULL);
	int i;
	for (i = 0; i < 4; i++)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
		{
			perror("Error while creating thread");
		}
	}
	for (i = 0; i < 4; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			perror("Error while joining thread");
		}
	}

	pthread_mutex_destroy(&mutex);
	return 0;
}
