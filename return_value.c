#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

void* roll_dice() {
	int value = (rand() % 6) + 1;
	int* result = (int*) malloc(sizeof(int));
	*result = value;
	//printf("value: %d\n", value);
	return (void*) result;
}

int main(int argc, char* argv[])
{
	int* res;
	pthread_t t;
	srand(time(NULL));
	
	if (pthread_create(&t, NULL, roll_dice, NULL) != 0)
	{
		return 1;
	}
	
	if (pthread_join(t, (void**) &res) != 0)
	{
		return 2;
	} 
	printf("Result: %d\n", *res);
	free(res);
	return 0;
}
