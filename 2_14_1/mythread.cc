#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;
string toHex(pthread_t tid)
{
    char hex[64];
    snprintf(hex, sizeof(hex), "%p", tid);
    return hex;
}
void *threadRoutine(void *args)
{
    while(1)
    {
        cout << "thread id: " << toHex(pthread_self()) << endl;
        sleep(1);
    }
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, nullptr);

    cout << "main thread create thead done, new thread id : " << toHex(tid) << endl;
    void *ret;
    pthread_join(tid, &ret);
    return 0;
}


// // 创建进程第四个参数还可以是一些特定的自定义类

// class Request
// {
// public:
//     Request(int start, int end, const string &threadname)
//     : start_(start), end_(end), threadname_(threadname)
//     {}
// public:
//     int start_;
//     int end_;
//     string threadname_;
// };
// class Response
// {
// public:
//     Response(int result, int exitcode):result_(result),exitcode_(exitcode)
//     {}
// public:
//     int result_;   // 计算结果
//     int exitcode_; // 计算结果是否可靠
// };
// void *threadRoutine(void *args)
// {
//     Request *rq = static_cast<Request*>(args); //等价于 Request *rq = (Request*)args，但是与之相比，其更安全
//     Response *rsp = new Response(0, 0);
//     for(int i = rq->start_; i <= rq->end_; i++)
//     {
//         cout << rq->threadname_ << " is runing, caling..., " << i << endl;
//         rsp->result_+=i;
//         usleep(100000);
//     }
//     delete rq;
//     return rsp;
// }
// int main()
// {
//     pthread_t tid;
//     Request *rq = new Request(1, 100, "thread 1");
//     pthread_create(&tid, nullptr, threadRoutine, rq);

//     void *ret;
//     pthread_join(tid, &ret);
//     Response *rsp = static_cast<Response *>(ret);
//     cout << "rsp->result: " << rsp->result_ << ", exitcode: " << rsp->exitcode_ << endl;
//     delete rsp;
//     return 0;
// }


// // 可以被多个执行流同时执行, show函数被重入了哦！
// void show(const string &name)
// {
//     cout << name << "say# "
//          << "hello thread" << endl;
// }

// int g_val = 100;
// // PTHREAD_CANCELED 是 POSIX 线程（pthread）库中的一个特殊常量，用于表示线程被取消的状态。与函数pthread_exit();设置退出状态
// // 是一个很大的数字
// void *threadRoutine(void *args)
// {
//     const char *name = (const char*)args;
//     int cnt = 5;
//     while(1)
//     {
//         printf("%s,  pid: %d, g_val: %d, &g_val: 0x%p\n", name,getpid(), g_val, &g_val);
//         sleep(1);
//         cnt--;
//         if(cnt==0) break;
//         // int a = 10;
//         // a /= 0;
//         show("[new thread]");
//         pthread_exit(PTHREAD_CANCELED);
//     }
//     return nullptr;
// }
// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, threadRoutine, (void*)"Thread 1");// 不是系统调用，底层是调用的clone()
//     show("[main thread]");

//     sleep(1);
//     // pthread_cancel(tid);// 不常用
//     // 俩都有按照运行顺序最后退出值为 -1

//     sleep(1);
//     void *retval;
//     pthread_join(tid, &retval);
//     // cout << "main thread quit ..., ret: " << (long long int)retval << endl;
//     if (retval == PTHREAD_CANCELED) 
//     {
//         printf("Thread was canceled.\n");
//     } 
//     else 
//     {
//         printf("Thread completed normally.\n");
//     }
//     return 0;
// }


// // 最简单的线程创建

// void *threadRun(void *args)
// {
//     int i;
//     while(1)
//     {
//         cout << "new thread: " << getpid() << endl;
//         sleep(1);
//     }
//     return nullptr;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, threadRun, nullptr);
//     int i;

//     while(1)
//     {
//         cout << "main thread: " << getpid() << endl;
//         sleep(1);
//     }
//     return 0;
// }