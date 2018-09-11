#include<pthread.h>
#include<stdlib.h>

extern int THREAD_COUNT;

//function to create pool. Returns 0 on success, non-zero value otherwise
void** createPool (void* , void* );

//function to join all threads
int joinThreads(pthread_t **);

//function to initilalize mutex lock
pthread_mutex_t* createMutex(void);

//function to clear allocated memory
int terminateThreads(pthread_t **);
