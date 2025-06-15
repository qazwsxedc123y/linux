#include <iostream>

#include <unistd.h>
#include <pthread.h>

using namespace std;

int tickets = 100;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *thread_run(void *arg)
{
    const char* name = (char*)arg;
	while (1)
    {
        pthread_mutex_lock(&mutex);
        while(tickets <= 0)  // 使用while而不是if
        {
            pthread_cond_wait(&cond, &mutex);
        }
        
        if (tickets > 0)
        {
            usleep(100000);  // 0.1秒
            printf("[%s] get a ticket, left: %d\n", name, --tickets);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
        usleep(3000);
	}
	printf("%s quit!\n", name);
	pthread_exit((void*)0);

}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t tid[5];
    for(int i = 0; i < 5; i++)
    {
        char* buffer = (char*)malloc(64);
        sprintf(buffer, "thread %d", i);
        pthread_create(&tid[i], NULL, thread_run, (void *)buffer);
    }

    while (tickets > 0)
    {
        getchar();
        pthread_cond_broadcast(&cond);
    }

    for(int i = 0; i < 5; i++)
    {
	    pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    
    return 0;
}