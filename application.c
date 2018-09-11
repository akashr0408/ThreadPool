#include<stdio.h>
#include "threadPool.h"

static const int RESOLUTION = 1000;

struct parameters		//parameters for thread scope function
{
	float llim, ulim;
	long double sub;
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
	p->sub = subArea; 
	printf("subArea = %.6f\n", subArea);
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

	//---------- create struct objects and thread ids---------
	struct parameters *p = malloc(THREAD_COUNT * sizeof *p);
	if(p == NULL)
	{
		printf("Malloc Failed!.... Exiting...");
		exit(1);
	}

	for(int i=0; i < THREAD_COUNT; i++)
	{
		p[i].llim = lLim + i*blockSize;
		p[i].ulim = p[i].llim + blockSize;
	}

	pthread_t **tID = malloc(sizeof **tID);
	if(tID == NULL)
	{
		printf("Not enough memory\n");
		exit(1);
	}
	
	tID = createPool(calcArea, p) //how to fucking parse arguments?

	joinThreads(*tID);
	long double area;
	for( int i =0; i<THREAD_COUNT; i++)
		area += p[i].sub;
		
	printf("Integration Result = %.6Lf", area);
	printf("\nDone!");
	free(p);
	terminate(tID);
	free(tID);
	return 0;
}
