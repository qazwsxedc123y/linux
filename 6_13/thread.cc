#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t mutex;
pthread_cond_t cond;

void *r1( void *arg)
{
    while ( 1 )
    {
        pthread_cond_wait(&cond, &mutex);
        printf("活动\n");
    }
}

void *r2(void *arg )
{
    while ( 1 ) 
    {
        pthread_cond_signal(&cond);
        sleep(1);
    }
}

int main()
{
	pthread_t t1, t2;
	pthread_mutex_init(&mutex, nullptr);
	pthread_cond_init(&cond, nullptr);
	
	pthread_create(&t1, nullptr, r1, (void*)"thread 1");
	pthread_create(&t2, nullptr, r2, (void*)"thread 2");
	

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}