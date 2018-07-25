#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

static const int THREAD_COUNT = 4;
static const int RESOLUTION = 100;

long double area = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


struct parameters
{
	int llim, ulim;
};

void* calcArea ( void *args)
{
	struct parameters* p = (struct parameters*) args;
	float stepSize = 1/RESOLUTION;
	printf("%d , %d", p->llim, p->ulim);
	printf("%.3f", stepSize);
	float subArea = 0;	
	for (float i =p->llim; i <= p->ulim; i+=stepSize)
	{
		subArea += (i*i)*stepSize;	//small rectangle  of height i^2 and width stepSize
		//printf("%d : %.3f\n", i, subArea);
	}
	
	printf("subArea = %.3f\n", subArea);		
	pthread_mutex_lock(&lock);
	area += subArea;
	pthread_mutex_unlock(&lock);
}

int main()
{
	int  lLim, uLim, retVal;
	float blockSize;		//integration range of each thread
	printf("Enter lower limit of integration: ");
	scanf("%d", &lLim);
	printf("\nEnter upper limit of integration: ");
	scanf("%d", &uLim);
	
	blockSize = (uLim - lLim)/THREAD_COUNT;
	
	//---------- create struct objects and thread ids---------
	struct parameters *p= malloc(sizeof(struct parameters)* THREAD_COUNT);
	pthread_t tID[4];
	
	for(int i=0; i<THREAD_COUNT; i++)
	{
		p[i].llim = lLim + i*blockSize;
		p[i].ulim = p[i].llim + blockSize;
		retVal = pthread_create(&tID[i], NULL, calcArea, &p[i]);
		pthread_join(tID[i], NULL);	
	}

	printf("Integration Result = %.3f", area);
	free(p);
}
