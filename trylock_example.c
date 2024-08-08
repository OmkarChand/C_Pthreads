#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutexFuel[4];
int stoveFuel[4] = { 100, 100, 100, 100 };

void* routine() 
{
	for (int i = 0; i < 4; i++)
	{
		if(pthread_mutex_trylock(&mutexFuel[i]) == 0)
		{
			int fuelNeeded = (rand() % 50);
			if (stoveFuel[i] - fuelNeeded < 0)
			{
				printf("No fuel, Going Home...\n");
			}
			else
			{
				stoveFuel[i] -= fuelNeeded;
				printf("Fuel left %d\n", stoveFuel[i]);
				usleep(500000);
			}
			pthread_mutex_unlock(&mutexFuel[i]);
			break;
		}
		else
		{
			if( i == 3)
			{
				printf("No sotve available yet, waiting...\n");
				usleep(300000);
				i = 0;
			}
		}
	}
}

int main()
{
	pthread_t th[10];
	srand(time(NULL));
	for (int i = 0; i < 4; i++){
		pthread_mutex_init(&mutexFuel[i], NULL);
	}

	for (int j = 0; j < 10; j++)
	{
		if (pthread_create(&th[j], NULL, &routine, NULL) != 0)
		{ perror("Failed to create thread"); }
	}

	for (int j = 0; j < 10; j++)
	{
		if (pthread_join(th[j], NULL) != 0)
		{ perror("Failed to join thread"); }
	}
	
	for (int i = 0; i < 4; i++){
		pthread_mutex_destroy(&mutexFuel[i]);
	}
	return 0;
}
