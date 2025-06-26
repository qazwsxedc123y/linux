#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
 
using namespace std;

class Sem{
public:
	Sem(int num)
	{
		sem_init(&_sem, 0, num);
	}
	~Sem()
	{
		sem_destroy(&_sem);
	}
	void P()
	{
		sem_wait(&_sem);
	}
	void V()
	{
		sem_post(&_sem);
	}
private:
	sem_t _sem;
};

int count = 100;
Sem sem(1); //二元信号量

void *thread_run(void *arg)
{
    const char* name = (char*)arg;
	while (1)
    {
        sem.P();
		if (count > 0)
        {
			sleep(0.1);
			printf("[%s] get a ticket, left: %d\n", name, --count);
            sem.V();
		}
		else
        {
            sem.V();
			break;
		}
	}
	printf("%s quit!\n", name);
	pthread_exit((void*)0);
 
}
 
int main()
{
    pthread_t tid[5];
    for(int i = 0; i < 5; i++)
    {
        char* buffer = (char*)malloc(64);
        sprintf(buffer, "thread %d", i);
        pthread_create(&tid[i], NULL, thread_run, (void *)buffer);
    }
 
    for(int i = 0; i < 5; i++)
    {
	    pthread_join(tid[i], NULL);
    }
 
 
    return 0;
}