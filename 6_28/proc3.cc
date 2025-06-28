#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define MAX_READERS 5
#define MAX_WRITERS 3

int read_count = 0;
int writer_wait_count = 0;
pthread_mutex_t mutex;         // 保护 count
sem_t queue_sem;               // 公平队列信号量
sem_t write_sem;               // 写者信号量（初始为1）
sem_t read_sem;                // 读者信号量（初始为1）

// 写者优先
// 当有写者等待时，新的读者不会插队

void *reader(void *arg)
{
    int id = *(int*)arg;
    while(1)
    {
        // 进入公平队列
        sem_wait(&queue_sem);

        // 检查是否有写者等待
        sem_wait(&read_sem);

        sem_post(&queue_sem);

        pthread_mutex_lock(&mutex);
        read_count++;
        if(read_count == 1)
        {
            sem_wait(&write_sem); // 第一个读者将写者阻塞
        }
        pthread_mutex_unlock(&mutex);

        sem_post(&read_sem); // 允许别的读者操作

        // 读取数据
        printf("Reader %d reading\n", id);

        pthread_mutex_lock(&mutex);
        read_count--;
        if(read_count == 0)
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
         // 进入公平队列
        sem_wait(&queue_sem);

        // 标记有写者等待
        pthread_mutex_lock(&mutex);
        writer_wait_count++;
        pthread_mutex_unlock(&mutex);

         // 离开队列，允许下一个线程
        sem_post(&queue_sem);
        

        sem_wait(&write_sem); // 获取写锁

        printf("Writer %d writing\n", id);

        printf("Writer %d write end\n", id);

        sem_post(&write_sem);

        pthread_mutex_lock(&mutex);
        writer_wait_count--;
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
}

int main()
{
    pthread_t readers[MAX_READERS], writers[2]; // 5个读者，2个写者
    int reader_ids[MAX_READERS], writer_ids[MAX_WRITERS];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&write_sem, 0, 1); // 初始写信号量为1（可进入）
    sem_init(&read_sem, 0, 1); // 初始写信号量为1（可进入）
    sem_init(&queue_sem, 0, 1);

    // 创建读者线程
    for (int i = 0; i < MAX_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // 创建写者线程
    for (int i = 0; i < MAX_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
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
    sem_destroy(&read_sem);
    return 0;
}