#include <iostream>
#include <pthread.h>
#include <cstring>

using namespace std;

pthread_mutex_t mutex;
pthread_cond_t cond;

void *thread_run(void *arg)
{
    const char *buffer = (char*)arg;
    while (1)
    {
        // 先加锁
        pthread_mutex_lock(&mutex);
        printf("%s\n",buffer);

        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t t1, t2;
    char* buffera = (char*)malloc(64);
    strcpy(buffera, "线程A打印-我是线程A");
    pthread_create(&t1, NULL, thread_run, buffera);
    char* bufferb = (char*)malloc(64);
    strcpy(bufferb, "线程B打印-我是线程B");
    pthread_create(&t2, NULL, thread_run, bufferb);


    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}