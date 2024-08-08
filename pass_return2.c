#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
int globalSum = 0;
pthread_mutex_t m;

void* routine(void* arg) {
        int index = *((int*)arg);
	//int localSum = 0;
	for (int j = 0; j < 5; j++)
	{
		pthread_mutex_lock(&m);
		globalSum += primes[index + j];
		pthread_mutex_unlock(&m);
	}
        //printf("Local sum: %d\n", localSum);
	// *(int*)arg = localSum;
	free(arg);
        return NULL;
}

int main (int argc, char* argv[])
{
        pthread_t t[2];
	pthread_mutex_init(&m, NULL);
        for (int i = 0; i < 2; i++)
        {
                int* a = (int*) malloc(sizeof(int));
                *a = i * 5;
                if (pthread_create(&t[i], NULL, &routine, a) != 0)
                { perror("Error while creating thread"); }
        }
	//int globalSum = 0;
        for (int i = 0; i < 2; i++)
        {
		//int* res;
                if (pthread_join(t[i], NULL) != 0)
                { perror("Error while joining thread"); }
		//globalSum += *res;
		//free(res);
        }
	
	printf("Global sum: %d\n", globalSum);
	pthread_mutex_destroy(&m);
        return 0;
}
