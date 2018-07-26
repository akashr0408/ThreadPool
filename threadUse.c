#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

static const int THREAD_COUNT = 6;
static const int RESOLUTION = 100;

long double area = 0;
pthread_mutex_t lock;


struct parameters		//parameters for thread scope function
{
	float llim, ulim;
};

void* calcArea ( void *args)
{
	struct parameters* _p = (struct parameters*) args;
	
	float stepSize = (float)1/RESOLUTION;
	
	//printf for debugging purposes
	printf("Ulim: %.2f , Llim: %.2f\n", _p->llim, _p->ulim);
	printf("stepsize: %.3f\n", stepSize);

	float subArea = 0;
	unsigned long int totRects = (_p->ulim - _p->llim)*RESOLUTION;	
	for(int i =0; i <= totRects; ++i)
	{
		float j = _p->llim + (float)i/RESOLUTION;
		subArea += (j*j)*stepSize;	//small rectangle  of height i^2 and width stepSize
	}
	
	printf("subArea = %.3f\n", subArea);		
	//pthread_mutex_lock(&lock);
	area += subArea;
	//pthread_mutex_unlock(&lock);
	printf("Area = %.3f\n", area);
	
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
	printf("Got those values \n");	
	blockSize = (float)(uLim - lLim)/THREAD_COUNT;
	//--printf for debugging
	printf("BlockSize as defined in main: %.3f\n", blockSize);

	//---------- create struct objects and thread ids---------
	struct parameters *p= malloc(sizeof(struct parameters)* THREAD_COUNT);
	if(p == NULL)
	{
		printf("Malloc Failed!.... Exiting...");
		exit(1);
	}
	printf("size of p : %.3f\n", (float)sizeof(p)/sizeof(struct parameters*));
	
	if(pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("Mutex failed..Exiting..");
		exit(1);
	}

	pthread_t tID[4];
	void *ret;
	
	for(int i=0; i<THREAD_COUNT; i++)
	{
		printf("In the for loop..\n");
		printf("i : %d\n", i);
		p[i].llim = lLim + i*blockSize;
		p[i].ulim = p[i].llim + blockSize;
		retVal = pthread_create(&tID[i], NULL, calcArea, &p[i]);
		if(retVal != 0)
		{
			printf("Thread not created");
			exit(2);
		}
		if(pthread_join(tID[i], &ret) != 0)
		{
			printf("Error Joining thread %d. Exiting...", i);
			exit(3);
		}
	}

	printf("Integration Result = %.3f", area);
	printf("Done!");
	free(p);
	pthread_mutex_destroy(&lock);
	
	return 0;
}
