#include "threadPool.h"

int THREAD_COUNT = 4;

void** createPool (void* func, void *args)
{
	pthread_t *tIDs = malloc(THREAD_COUNT * sizeof *tIDs);
	int retVal;
	if(tIDs == NULL)
	{
		return NULL;
	}
	else
	{
		for(int i=0; i< THREAD_COUNT; i++)
		{
			retVal = pthread_create(&tIDs[i],NULL,func, args);
			if(retVal != 0)
			{
				return NULL;
			}
		}
		return &tIDs;
	}
}

int joinThreads(pthread_t** tIDs)
{
	for(int i=0; i< THREAD_COUNT; i++)
	{
		if(pthread_join(*tIDs[i], NULL) != 0)
		{
			return 1;
		}
	}
	return 0;
}

pthread_mutex_t* createMutex(void)
{
	pthread_mutex_t *paramLock = malloc(sizeof *paramLock);
	int success = pthread_mutex_init(paramLock, NULL);
	if (success == 1)
		return paramLock;
	else 
		return NULL;
}


int terminate(pthread_t** tIDs)
{
	free(*tIDs);
	return 0;
}

