#include <iostream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include "LockGuard.hpp"

using namespace std;

#define NUM 4
int tickets = 1000; // 用多线程，模拟一轮抢票
// pthread_mutex_t lock; // 动态分配与函数 pthread_mutex_init 配用
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;// 静态分配（初始化互斥量）

class threadData
{
public:
    threadData(int number, pthread_mutex_t *mutex)
    {
        threadname = "thread-" + to_string(number);
        lock = mutex;
    }
public:
    string threadname;
    pthread_mutex_t *lock;
};


// 封装版

void *getTicket(void *args)
{
    threadData *td = static_cast<threadData *>(args);
    const char *name = td->threadname.c_str();
    while (true)
    {
        LockGuard lockguard(&lock); // 临时的LockGuard对象， RAII风格的锁！
        if(tickets > 0)
        {
            usleep(1000);
            printf("who=%s, get a ticket: %d\n", name, tickets); // ?
            tickets--;
        }
        else
        {
            break;
        }
        usleep(13); // 我们抢到了票，我们会立马抢下一张吗？其实多线程还要执行得到票之后的后续动作。usleep模拟
    }
    printf("%s ... quit\n", name);
    return nullptr;
}


// void *getTicket(void *args)
// {
//     threadData *td = static_cast<threadData *>(args);
//     const char *name = td->threadname.c_str();
//     while (true)
//     {
//         // 放着不放 while 循环外是因为效率
//         // 设置锁的原因是因为不同线程的竞争能力不同
//         // pthread_mutex_lock(td->lock); // 申请锁成功，才能往后执行，不成功，阻塞等待。

//         // pthread_mutex_lock(&lock);
//         pthread_mutex_lock(td->lock);
//         if(tickets > 0)
//         {
//             usleep(1000);
//             printf("who=%s, get a ticket: %d\n", name, tickets); // ?
//             tickets--;
//             // pthread_mutex_unlock(&lock); 
//             pthread_mutex_unlock(td->lock);
//         }
//         else
//         {
//             // pthread_mutex_unlock(&lock);
//             pthread_mutex_unlock(td->lock);
//             break;
//         }
//         usleep(13); // 我们抢到了票，我们会立马抢下一张吗？其实多线程还要执行得到票之后的后续动作。usleep模拟
//     }
//     printf("%s ... quit\n", name);
//     return nullptr;
// }

int main()
{
    // pthread_mutex_init(&lock, nullptr); // 初始化动态锁


    vector<pthread_t> tids;
    vector<threadData*> thread_datas;
    for(int i = 1; i <= NUM; i++)
    {
        pthread_t tid;
        threadData* td = new threadData(i, &lock);
        thread_datas.push_back(td);
        pthread_create(&tid, nullptr, getTicket, thread_datas[i - 1]);
        tids.push_back(tid);
    }

    for(auto thread : tids)
    {
        pthread_join(thread, nullptr);
    }

    for (auto td : thread_datas)
    {
        delete td;
    }

    // 最后要记得销毁互斥量
    // pthread_mutex_destroy(&lock);

    return 0;
}