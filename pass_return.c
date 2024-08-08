#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void* routine(void* arg) {
        int index = *((int*)arg);
	int localSum = 0;
	for (int j = 0; j < 5; j++)
	{
		localSum += primes[index + j];
	}
        printf("Local sum: %d\n", localSum);
	*(int*)arg = localSum;
        return arg;
}

int main (int argc, char* argv[])
{
        pthread_t t[2];

        for (int i = 0; i < 2; i++)
        {
                int* a = (int*) malloc(sizeof(int));
                *a = i * 5;
                if (pthread_create(&t[i], NULL, &routine, a) != 0)
                { perror("Error while creating thread"); }
        }
	int globalSum = 0;
        for (int i = 0; i < 2; i++)
        {
		int* res;
                if (pthread_join(t[i], (void**) &res) != 0)
                { perror("Error while joining thread"); }
		globalSum += *res;
		free(res);
        }
	
	printf("Global sum: %d\n", globalSum);
        return 0;
}
