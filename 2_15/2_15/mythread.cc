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
pthread_mutex_t lock; // 动态分配与函数 pthread_mutex_init 配用
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;// 静态分配（初始化互斥量）

pthread_cond_t cond; // 条件变量，用于同步， 要与函数pthread_cond_inti 初始化
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;// 不需要与函数初始化

class threadData
{
public:
    threadData(int number /* , pthread_mutex_t *mutex */)
    {
        threadname = "thread-" + to_string(number);
        //lock = mutex;
    }
public:
    string threadname;
    // pthread_mutex_t *lock;
};


// // 封装版

// void *getTicket(void *args)
// {
//     threadData *td = static_cast<threadData *>(args);
//     const char *name = td->threadname.c_str();
//     while (true)
//     {
//         LockGuard lockguard(&lock); // 临时的LockGuard对象， RAII风格的锁！
//         if(tickets > 0)
//         {
//             usleep(1000);
//             printf("who=%s, get a ticket: %d\n", name, tickets); // ?
//             tickets--;
//         }
//         else
//         {
//             break;
//         }
//         usleep(13); // 我们抢到了票，我们会立马抢下一张吗？其实多线程还要执行得到票之后的后续动作。usleep模拟
//     }
//     printf("%s ... quit\n", name);
//     return nullptr;
// }


void *getTicket(void *args)
{
    threadData *td = static_cast<threadData *>(args);
    const char *name = td->threadname.c_str();
    while (true)
    {
        // 放着不放 while 循环外是因为效率
        // 设置锁的原因是因为不同线程的竞争能力不同
        // pthread_mutex_lock(td->lock); // 申请锁成功，才能往后执行，不成功，阻塞等待。

        pthread_mutex_lock(&lock);
        // pthread_mutex_lock(td->lock);


        // 我们怎么知道我们要让一个线程去休眠了那？一定是临界资源不就绪，没错，临界资源也是有状态的！！
        // 你怎么知道临界资源是就绪还是不就绪的？你判断出来的！判断是访问临界资源吗？必须是的，也就是判断必须在加锁之后！！！
        pthread_cond_wait(&cond, &lock);//？ 为什么在这里？ 1. pthread_cond_wait让线程等待的时候，会自动释放锁！
        if(tickets > 0)
        {
            usleep(1000);
            printf("who=%s, get a ticket: %d\n", name, tickets); // ?
            tickets--;
            pthread_mutex_unlock(&lock); 
            // pthread_mutex_unlock(td->lock);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            // pthread_mutex_unlock(td->lock);
            break;
        }
        // usleep(13); // 我们抢到了票，我们会立马抢下一张吗？其实多线程还要执行得到票之后的后续动作。usleep模拟
    }
    printf("%s ... quit\n", name);
    return nullptr;
}

int main()
{
    pthread_mutex_init(&lock, nullptr); // 初始化动态锁

    pthread_cond_init(&cond, nullptr); // 初始化条件变量 


    vector<pthread_t> tids;
    vector<threadData*> thread_datas;
    for(int i = 1; i <= NUM; i++)
    {
        pthread_t tid;
        threadData* td = new threadData(i/* , &lock */);
        thread_datas.push_back(td);
        pthread_create(&tid, nullptr, getTicket, thread_datas[i - 1]);
        tids.push_back(tid);
    }

    while(true) 
    {
        sleep(1);
        pthread_cond_signal(&cond); //唤醒在cond的等待队列中等待的一个线程，默认都是第一个
        // pthread_cond_broadcast(&cond);
        std::cout << "signal one thread..." << std::endl;
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
    pthread_mutex_destroy(&lock);

    // 销毁条件变量
    pthread_cond_destroy(&cond);

    return 0;
}