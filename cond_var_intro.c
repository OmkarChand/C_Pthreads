#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;

int fuel = 0;

void* filling_station(void* arg)
{
	for (int i = 0; i < 5; i++)
	{
		pthread_mutex_lock(&mutexFuel);
		fuel += 15;
		printf("Filled fuel in station... %d\n", fuel);
		pthread_mutex_unlock(&mutexFuel);
		pthread_cond_signal(&condFuel);
		sleep(1);
	}
}

void* filling_car()
{
	pthread_mutex_lock(&mutexFuel);
	while (fuel < 40)
	{	printf("NO fuel, Waiting...\n");
		pthread_cond_wait(&condFuel, &mutexFuel);
	}
	fuel -= 40;
	printf("Got fuel. Remaining fuel: %d\n",  fuel);
	pthread_mutex_unlock(&mutexFuel);
}

int main (int argc, char* argv[])
{
	pthread_t th[2];
	pthread_mutex_init(&mutexFuel, NULL);
	pthread_cond_init(&condFuel, NULL);

	for (int i = 0; i < 2; i++)
	{
		if (i == 1)
		{
			if (pthread_create(&th[i], NULL, &filling_station, NULL) != 0)
			{ perror("Failed to create thread"); }
		}
		else
		{
			if (pthread_create(&th[i], NULL, &filling_car, NULL) != 0)
			{ perror("Failed to create thread"); }
		}
	}
	for (int i = 0; i < 2; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{ perror("Failed to join thread"); }
	}

	pthread_mutex_destroy(&mutexFuel);
	pthread_cond_destroy(&condFuel);
	return 0;
}
