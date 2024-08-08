#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#define THREAD_NUM 4

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

typedef struct Task {
	void (*taskFunction) (int, int); 
	int a, b;
} Task;

Task taskQueue[256];
int taskCount = 0;

// void sumAndProduct(int a, int b) {
//	usleep(50000);
//	int sum = a + b;
//	int prod = a * b;
//	printf("Sum and product of %d and %d is %d and %d respectively\n", a, b, sum, prod);
// }

void sum (int a, int b) {
	usleep(50000);
	int sum = a + b;
	printf("The sum of %d and %d is %d\n", a, b, sum);
}

void product (int a, int b) {
	usleep(50000);
	int prod = a * b;
	printf("The product of %d and %d is %d\n", a, b, prod);
}

void executeTask (Task* task) {
	task->taskFunction (task->a, task->b);
	// usleep(50000);
	// int result = task->a + task->b;
	// printf("The sum of %d and %d is %d\n", task->a, task->b, result);
}

void submitTask (Task task) {
	pthread_mutex_lock(&mutexQueue);
	taskQueue[taskCount] = task;
	taskCount++;
	pthread_mutex_unlock(&mutexQueue);
	pthread_cond_signal(&condQueue); // we are signaling the conditional variable here because whenever a task is submitted 
					 // then only we know that a task is added in the queue
					 // we are not using broadcast here because one thread will execute one task only
}

void* startThread (void* args) {
	while (1) {
		
		Task task;
		// int found = 0; // whatever is comment down here is because we are using conditional variable to check
				  // is there any task in the queue or not, hence we don't need to busy wait (High CPU utilization)
				  // instead conditional variable will wokeup it when there exist a task in queue
		
		pthread_mutex_lock(&mutexQueue);
		while (taskCount == 0) {
			pthread_cond_wait(&condQueue, &mutexQueue);
		}
		
		// if (taskCount > 0)
		// {
		//	found = 1;
			task = taskQueue[0];
			for (int i = 0; i < taskCount; i++)
			{
				taskQueue[i] = taskQueue[i+1];
			}
			taskCount--;
		// }
		pthread_mutex_unlock(&mutexQueue);
		
		// if (found == 1)
			executeTask(&task);
	}
}

int main (int argc, char* argv[]){
	pthread_t th[THREAD_NUM];
	pthread_mutex_init(&mutexQueue, NULL);
	pthread_cond_init(&condQueue, NULL);
	
	int i;
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_create(&th[i], NULL, &startThread, NULL) != 0)
		{	perror("Failed to create thread\n");	}
	}
	
	srand(time(NULL));
	for (i = 0; i < 100; i++)
	{
		Task t = {
			// .taskFunction = &sumAndProduct,
			.taskFunction = (i % 2 == 0 ? &sum : &product),
			.a = rand() % 100,
			.b = rand() % 100
		};
		submitTask(t);
	}	
	
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{	perror("Failed to join thread\n");	}
	}
	
	pthread_mutex_destroy(&mutexQueue);
	pthread_cond_destroy(&condQueue);
	return 0;
}

