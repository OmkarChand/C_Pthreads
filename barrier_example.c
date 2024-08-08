#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_NUM 8

int dice_values [THREAD_NUM];
int status[THREAD_NUM] = { 0 };

pthread_barrier_t barrierRollDice;
pthread_barrier_t barrierCalculate;

void* roll(void* arg)
{
 int index = *(int*)arg;
 while (1)
 {
	dice_values[index] = (rand() % 6 + 1);
	pthread_barrier_wait(&barrierRollDice);
	pthread_barrier_wait(&barrierCalculate);
	if(status[index] == 1)
	{ printf("(%d rolled %d), I won\n", index, dice_values[index]); }
	else
	{ printf("(%d rolled %d), I lost\n", index, dice_values[index]); }
 }
	free(arg);
}

int main(int argc, char* argv[])
{
	pthread_t th[THREAD_NUM];
	srand(time(NULL));

	pthread_barrier_init(&barrierRollDice, NULL, THREAD_NUM + 1);
	pthread_barrier_init(&barrierCalculate, NULL, THREAD_NUM + 1);

	int i;
	for (i = 0; i < THREAD_NUM; i++)
	{
		int* a = (int*) malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &roll, a) != 0)
		{ perror("Failed to create thread"); }
	}

 while (1) {
	pthread_barrier_wait(&barrierRollDice);
	int max = 0;
	for(int j = 0; j < THREAD_NUM; j++)
	{
		if(max < dice_values[j])
		{ max = dice_values[j]; }
	}
	
	for(int j = 0; j < THREAD_NUM; j++)
	{
		if (dice_values[j] == max)
			status[j] = 1;
		else
			status[j] = 0;
	}
	sleep(1);
	printf("=== New round starting ===\n");
	pthread_barrier_wait(&barrierCalculate);
 }
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{ perror("Failed to join thread"); }
	}
	pthread_barrier_destroy(&barrierRollDice);
	pthread_barrier_destroy(&barrierCalculate);
	return 0;
}
