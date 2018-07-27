#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

static const int THREAD_COUNT = 4;
static const int RESOLUTION = 1000;
long double area = 0;
pthread_mutex_t areaLock = PTHREAD_MUTEX_INITIALIZER;

struct parameters		//parameters for thread scope function
{
	float llim, ulim;
};

void* calcArea ( void *args)
{
	struct parameters* _p = (struct parameters*) args;
	float stepSize = (float)1/RESOLUTION;
	double subArea = 0;
	unsigned long int totRects = (_p->ulim - _p->llim)*RESOLUTION;
	for(int i =0; i <= totRects; ++i)
	{
		float j = _p->llim + (float)i/RESOLUTION;
		subArea += (j*j)*stepSize;	//small rectangle  of height i^2 and width stepSize
	}
	printf("subArea = %.6f\n", subArea);
	pthread_mutex_lock(&areaLock);
	area += subArea;
	pthread_mutex_unlock(&areaLock);
	return NULL;
}

int main()
{
	int  lLim, uLim, retVal;
	float blockSize;		//integration range of each thread
	printf("Enter lower limit of integration: ");
	scanf("%d", &lLim);
	printf("\nEnter upper limit of integration: ");
	scanf("%d", &uLim);
	blockSize = (float)(uLim - lLim)/THREAD_COUNT;
	//--printf for debugging
	printf("BlockSize as defined in main: %.3f\n", blockSize);

	//---------- create struct objects and thread ids---------
	struct parameters *p = malloc(THREAD_COUNT * sizeof *p);
	if(p == NULL)
	{
		printf("Malloc Failed!.... Exiting...");
		exit(1);
	}
	pthread_t tID[THREAD_COUNT];
	void *ret;

	for(int i=0; i < THREAD_COUNT; i++)
	{
		p[i].llim = lLim + i*blockSize;
		p[i].ulim = p[i].llim + blockSize;
		retVal = pthread_create(&tID[i], NULL, calcArea, &p[i]);
		if(retVal != 0)
		{
			printf("Thread not created");
			exit(2);
		}
	}

	for(int i=0; i < THREAD_COUNT; i++)
	{
		if(pthread_join(tID[i], &ret) != 0)
		{
			printf("Error Joining thread %d. Exiting...", i);
			exit(3);
		}
	}

	printf("Integration Result = %.6Lf", area);
	printf("\nDone!");
	free(p);
	pthread_mutex_destroy(&areaLock);

	return 0;
}
