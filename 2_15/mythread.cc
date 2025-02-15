#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#define NUM 4

class threadData
{
public:
    threadData(int number /*, pthread_mutex_t *mutex*/)
    {
        threadname = "thread-" + to_string(number);
        // lock = mutex;
    }

public:
    string threadname;
    // pthread_mutex_t *lock;
};

int tickets = 3000; // 用多线程，模拟一轮抢票

void *getTicket(void *args)
{
    threadData *td = static_cast<threadData *>(args);
    const char *name = td->threadname.c_str();
    while (true)
    {
        // 线程对于锁的竞争能力可能会不同 --- 一会由例子
        // pthread_mutex_lock(td->lock); // 申请锁成功，才能往后执行，不成功，阻塞等待。
        pthread_mutex_lock(&lock); // 申请锁成功，才能往后执行，不成功，阻塞等待。
        pthread_mutex_lock(&lock); // 申请锁成功，才能往后执行，不成功，阻塞等待。
        if(tickets > 0)
        {
            usleep(1000);
            printf("who=%s, get a ticket: %d\n", name, tickets); // ?
            tickets--;
            // pthread_mutex_unlock(td->lock);
            // pthread_mutex_unlock(&lock);
        }
        else{
            // pthread_mutex_unlock(td->lock);
            // pthread_mutex_unlock(&lock);

            break;
        }
        usleep(13); // 我们抢到了票，我们会立马抢下一张吗？其实多线程还要执行得到票之后的后续动作。usleep模拟
    }
    printf("%s ... quit\n", name);
    return nullptr;
}

// void fun()
// {
//     static int cnt = 0;
//     cnt++;
//     printf("hello fun()\n");
// }

// void *getTicket(void *args)
// {
//     threadData *td = static_cast<threadData *>(args);
//     const char *name = td->threadname.c_str();
//     while (true)
//     {
//         {
//             LockGuard lockguard(&lock); // 临时的LockGuard对象， RAII风格的锁！
//             if (tickets > 0)
//             {
//                 usleep(1000);
//                 printf("who=%s, get a ticket: %d\n", name, tickets); // ?
//                 tickets--;
//             }
//             else
//                 break;
//         }
//         usleep(13); // 我们抢到了票，我们会立马抢下一张吗？其实多线程还要执行得到票之后的后续动作。usleep模拟
//     }
//     printf("%s ... quit\n", name);
//     return nullptr;
// }

int main()
{
    // pthread_mutex_init(&lock, nullptr);

    vector<pthread_t> tids;
    vector<threadData *> thread_datas;
    for (int i = 1; i <= NUM; i++)
    {
        pthread_t tid;
        threadData *td = new threadData(i /*, &lock*/);
        thread_datas.push_back(td);
        pthread_create(&tid, nullptr, getTicket, thread_datas[i - 1]);
        tids.push_back(tid);
    }

    for (auto thread : tids)
    {
        pthread_join(thread, nullptr);
    }

    for (auto td : thread_datas)
    {
        delete td;
    }

    // pthread_mutex_destroy(&lock);
    return 0;
}

// #define NUM 3

// // int *p = NULL;

// // __thread int g_val = 100;
// __thread unsigned int number = 0;
// __thread int pid = 0;

// struct threadData
// {
//     string threadname;
// };

// // __thread threadData td;

// string toHex(pthread_t tid)
// {
//     char buffer[128];
//     snprintf(buffer, sizeof(buffer), "0x%x", tid);
//     return buffer;
// }

// void InitThreadData(threadData *td, int number)
// {
//     td->threadname = "thread-" + to_string(number); // thread-0
// }

// // 所有的线程，执行的都是这个函数？
// void *threadRoutine(void *args)
// {
//     pthread_detach(pthread_self());

//     // int test_i = 0;
//     threadData *td = static_cast<threadData *>(args);
//     // if(td->threadname == "thread-2") p = &test_i;
//     string tid = toHex(pthread_self());
//     int pid = getpid();

//     int i = 0;
//     while (i < 10)
//     {
//         cout << "tid: " << tid << ", pid: " << pid << endl;

//         // cout << "pid: " << getpid() << ", tid : "
//         //     << toHex(number) << ", threadname: " << td->threadname
//         //         << ", g_val: " << g_val << " ,&g_val: " << &g_val <<endl;
//         sleep(1);
//         i++;
//     }

//     delete td;
//     return nullptr;
// }

// int main()
// {
//     // 创建多线程！
//     vector<pthread_t> tids;
//     for (int i = 0; i < NUM; i++)
//     {
//         pthread_t tid;
//         threadData *td = new threadData;
//         InitThreadData(td, i);

//         pthread_create(&tid, nullptr, threadRoutine, td);
//         tids.push_back(tid);
//         //sleep(1);
//     }
//     sleep(1); // 确保复制成功

//     // for(auto i : tids)
//     // {
//     //     pthread_detach(i);
//     // }
//     // cout << "main thread get a thread local value, val: " << *p << ", &val: " << p << endl;

//     for (int i = 0; i < tids.size(); i++)
//     {
//         int n = pthread_join(tids[i], nullptr);
//         printf("n = %d, who = 0x%x, why: %s\n", n, tids[i], strerror(n));
//     }

//     return 0;
// }