#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

void* roll_dice() {
	int value = (rand() % 6) + 1;
	int* result = (int*) malloc(sizeof(int));
	*result = value;
	sleep(1);
	printf("value: %d\n", value);
	// return (void*) result;
	pthread_exit((void*) result); // Another way to return the value from the thread
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
	
	// pthread_exit(0); // if you use this inside the main()
			 // then it doesnot terminate main()
			 // but it wait for the thread to finish
	
	if (pthread_join(t, (void**) &res) != 0)
	{
		return 2;
	} 
	printf("Result: %d\n", *res);
	free(res);
	return 0;
}
