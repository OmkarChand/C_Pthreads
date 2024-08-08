
// Using this way you can directly create a detached thread instead of creating and detaching it from main thread

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_NUM 2

void* routine(void* arg)
{
	sleep(1);
	printf("Finished execution\n");
}

int main(int argc, char* argv[])
{
	pthread_t th[THREAD_NUM];
	pthread_attr_t detachedThread;
	pthread_attr_init(&detachedThread);
	pthread_attr_setdetachstate(&detachedThread, PTHREAD_CREATE_DETACHED);
	
	int i;
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_create(&th[i], &detachedThread, &routine, NULL) != 0)
		{	perror("Failed to create thread");	}
	}
	//for (i = 0; i < THREAD_NUM; i++)	// since threads are detached they can not be joined
	//{ 
	//	if (pthread_join(th[i], NULL) != 0)
	//	{	perror("Failed to join thread");	}
	//}
	
	// return 0; // if you use return 0 then the main program (or main thread) stops execution
		  // and since thread are detached, they may terminated from execution without completion
	
	pthread_attr_destroy(&detachedThread);
	pthread_exit(0); // if you use this instead of return 0 or exit(0) then the main program terminate after
			 // completion but the threads also finish its complete execution
}
