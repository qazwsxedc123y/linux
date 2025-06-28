#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define MAX_READERS 5

int count = 0;          // 当前活跃的读者数量
pthread_mutex_t mutex;         // 保护 count
sem_t write_sem;               // 写者信号量（初始为1）

void *reader(void *arg)
{
    int id = *(int*)arg;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        count++;
        if(count == 1)
        {
            sem_wait(&write_sem); // 第一个读者将写者阻塞
        }
        pthread_mutex_unlock(&mutex);

        // 读取数据
        printf("Reader %d reading\n", id);

        pthread_mutex_lock(&mutex);
        count--;
        if(count == 0)
        {
            sem_post(&write_sem);
        }
        printf("Reader %d read end\n", id);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
}

void *writer(void *arg)
{
    int id = *(int*)arg;
    while (1)
    {
        sem_wait(&write_sem);

        printf("Writer %d writing\n", id);

        printf("Writer %d write end\n", id);

        sem_post(&write_sem);

        sleep(1);
    }
}

int main()
{
    pthread_t readers[MAX_READERS], writers[2]; // 5个读者，2个写者
    int ids[MAX_READERS];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&write_sem, 0, 1); // 初始写信号量为1（可进入）

    // 创建读者线程
    for (int i = 0; i < MAX_READERS; i++) {
        ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &ids[i]);
    }

    // 创建写者线程
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, &ids[i]);
    }

    // 等待线程结束
    for (int i = 0; i < MAX_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&write_sem);
    return 0;
}