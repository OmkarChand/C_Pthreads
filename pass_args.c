#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int primes[] = { 2, 3, 5, 7, 9, 11, 13, 17, 23, 29  };

void* routine(void* arg) {
	int index = *((int*)arg);
	printf("%d ", primes[index]);
	free(arg);
	return NULL;
}

int main (int argc, char* argv[])
{
	pthread_t t[10];
	
	for (int i = 0; i < 10; i++)
	{
		int* a = (int*) malloc(sizeof(int));
		*a = i;
		if (pthread_create(&t[i], NULL, routine, (void*) a) != 0)
		{ printf("Error while creating thread"); }
	}

	for (int i = 0; i < 10; i++)
	{
		if (pthread_join(t[i], NULL) != 0)
		{ printf("Error while joining thread"); }
	}
	return 0;
}
