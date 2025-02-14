#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <cstdio>

using namespace std;

#define NUM 4
class threadData
{
public:
    threadData(int number)
    {
        threadname = "thread-" + to_string(number);
    }

public:
    string threadname;
};
string toHex(pthread_t tid)
{
    char hex[128];
    snprintf(hex, sizeof(hex), "0x%x", tid);
    return hex;
}

// 多人抢票系统模拟
// 多进程是多人

// 但是有问题，最后票数会 -- 到负数
// 下次修正

int tickets = 1000; // 用多线程，模拟一轮抢票

void *getTicket(void *args)
{
    threadData *td = static_cast<threadData *>(args);
    const char *name = td->threadname.c_str();
    while (1)
    {
        if (tickets > 0)
        {
            usleep(1000);
            printf("who=%s, get a ticket: %d\n", name, tickets);
            tickets--;
        }
        else
        {
            break;
        }
    }
    printf("%s ... quit\n", name);
    return nullptr;
}
int main()
{
    vector<pthread_t> tids;
    vector<threadData *> thread_datas;
    for (int i = 1; i <= NUM; i++)
    {
        pthread_t tid;
        threadData *td = new threadData(i);
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
    return 0;
}

// int *p = NULL;

// __thread int g_val = 100;
// __thread unsigned int number = 0;
// __thread int pid = 0;

// // 所有的线程，执行的都是这个函数？   --  对
// void *threadRoutine(void *args)
// {
//     pthread_detach(pthread_self());

//     int test_i = 0;
//     threadData *td = static_cast<threadData *>(args);
//     if(td->threadname == "thread-2") p = &test_i;
//     string tid = toHex(pthread_self());
//     int pid = getpid();

//     int i = 0;
//     while (i < 10)
//     {
//         cout << "pid: " << getpid() << ", tid : "
//             << toHex(number) << ", threadname: " << td->threadname
//                 << ", g_val: " << g_val << " ,&g_val: " << &g_val <<endl;
//         sleep(1);
//         i++;
//     }

//     delete td;
//     return nullptr;
// }
// int main()
// {
//     vector<pthread_t> tids; // 保存所有进程的 ID
//     for (int i = 0; i < NUM; i++)
//     {
//         pthread_t tid;
//         threadData *td = new threadData(i);

//         pthread_create(&tid, nullptr, threadRoutine, td);
//         tids.push_back(tid);
//     }

//     // // 分离后，就不用join了，在join就会出错，但再分离还是线程，还共享资源，只是别人不管你了
//     // for (auto i : tids)
//     // {
//     //     pthread_detach(i);
//     // }
//     // cout << "main thread get a thread local value, val: " << *p << ", &val: " << p << endl;

//     // 等待所有线程完成
//     for (auto tid : tids)
//     {
//         pthread_join(tid, nullptr);
//     }
//     return 0;
// }

// 分代码一，观察看线程的调度顺序

// void *threadRoutine(void *args)
// {
//     threadData *td = static_cast<threadData *>(args);
//     pthread_detach(pthread_self());// 同样也可以自己为自己分离
//     cout<<td->threadname<<endl;
//     return nullptr;
// }
// int main()
// {
//     vector<pthread_t> tids; // 保存所有进程的 ID
//     for (int i = 0; i < NUM; i++)
//     {
//         pthread_t tid;
//         threadData *td = new threadData(i);

//         pthread_create(&tid, nullptr, threadRoutine, td);
//         tids.push_back(tid);
//         cout<<i<<endl;
//     }

//     // // 分离后，就不用join了，在join就会出错，但再分离还是线程，还共享资源，只是别人不管你了
//     // for (auto i : tids)
//     // {
//     //     pthread_detach(i);
//     // }
//     // cout << "main thread get a thread local value, val: " << *p << ", &val: " << p << endl;

//     cout<<"6"<<endl;

//     // 等待所有线程完成
//     for (auto tid : tids)
//     {
//         pthread_join(tid, nullptr);
//     }
//     return 0;
// }