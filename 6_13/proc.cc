#include <iostream>

#include <unistd.h>
#include <pthread.h>

using namespace std;

int tickets = 100;
pthread_mutex_t mutex;

void *thread_run(void *arg)
{
    const char* name = (char*)arg;
	while (1)
    {
        pthread_mutex_lock(&mutex);
		if (tickets > 0)
        {
			sleep(0.1);
			printf("[%s] get a ticket, left: %d\n", name, --tickets);
            pthread_mutex_unlock(&mutex);
		}
		else
        {
            pthread_mutex_unlock(&mutex);
			break;
		}
	}
	printf("%s quit!\n", name);
	pthread_exit((void*)0);

}

int main()
{
    pthread_mutex_init(&mutex, NULL);
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

    pthread_mutex_destroy(&mutex);
    return 0;
}