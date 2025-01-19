#include <iostream>
#include <string>
#include <pthread.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

using namespace std;
string toHex(pthread_t tid)
{
    char hex[64];
    snprintf(hex, sizeof(hex), "%p", tid);
    return hex;
}
// pthread_self获取线程的 ID
void *threadRoutine(void *args)
{
    while(true)
    {
        cout << "thread id: " << toHex(pthread_self()) << endl;
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, (void*)"thread 1");

    cout << "main thread create thead done, new thread id : " << toHex(tid) << endl;
    pthread_join(tid, nullptr);

    return 0;
}







// 目前，我们的原生线程，pthread库，原生线程库
// // C++11 语言本身也已经支持多线程了 vs 原生线程库




// 对于第四个参数不仅可以穿一般的参数，还可以传一些复杂的参数，比如类对象

// class Request
// {
// public:
//     Request(int start, int end, const string &threadname)
//         : start_(start), end_(end), threadname_(threadname)
//     {}
// public:
//     int start_;
//     int end_;
//     string threadname_;
// };

// class Response
// {
// public:
//     Response(int result, int exitcode)
//         :result_(result),exitcode_(exitcode)
//     {}
// public:
//     int result_;   // 计算结果
//     int exitcode_; // 计算结果是否可靠
// };

// void *sumCount(void* args)
// {
//     Request *rq = static_cast<Request*>(args); //  Request *rq = (Request*)args
//     Response *rsp = new Response(0,0);
//     for(int i = rq->start_; i <= rq->end_; i++)
//     {
//         cout << rq->threadname_ << " is runing, caling..., " << i << endl;
//         rsp->result_ += i;
//         usleep(100000);
//     }
//     delete rq;
//     return static_cast<void*>(rsp);
// }

// int main()
// {
//     pthread_t tid;
//     Request *rq = new Request(1, 100, "thread 1");

//     pthread_create(&tid, nullptr, sumCount, rq);


//     void *ret;
//     pthread_join(tid, &ret);

//     Response *rsp = static_cast<Response *>(ret);
//     cout << "rsp->result: " << rsp->result_ << ", exitcode: " << rsp->exitcode_ << endl;
//     delete rsp;
//     return 0;
// }





// void *threadRoutine(void *args)
// {
//     int cnt=0;
//     const char *name = (const char*)args;
//     int cnt1 = 5;
//     while(true)
//     {
//         printf("%s, pid is:%d, cnt is:%d\n",name,getpid(),cnt1);
//         sleep(1);
//         cnt1--;


//         // 异常也会使得该线程（执行流）退出
//         int a = 10;
//         a /= 0;


//         if(cnt1==0) break;
//     }
//     // exit(11); // 直接调用exit, exit是用来终止进程的！！，不能用来直接终止进程！所以无法打印最后的那个退出信息
//     // pthread_exit((void*)100); // 应该用这个函数
//     return (void*)100; //走到这里默认线程退出了！
//     // return nullptr;
// }

// int main()
// {
//     // 调用函数 pthread_create 创建线程
//     pthread_t tid;
//     pthread_create(&tid, nullptr, threadRoutine, (void*)"thread 1");
//     sleep(1); // 只是为了保证新线程已经启动


//     pthread_cancel(tid); // 不常见 ,也是让线程退出
//     int cnt = 0;
//     while(true)
//     {
//         printf("main thread, pid is:%d, cnt is:%d\n",getpid(),cnt);
//         sleep(1);
//         cnt++;
//         if(cnt==5) break;
//     }

//     // 使用 pthread_join 等待子线程退出
//     void *retval;
//     if (pthread_join(tid, &retval) != 0)
//     {
//         perror("pthread_join failed");
//         return -1;
//     }

//     cout << "main thread quit ..., ret: " << (long long int)retval << endl;
//     return 0;
// }







// 最简单的线程的创建

// void *threadRoutine(void *args)
// {
//     printf("new thread, pid is:%d\n",getpid());
//     // cout << "new thread, pid is:" << getpid() << endl;
//     return nullptr;
// }

// int main()
// {
//     // 调用函数 pthread_create 创建线程
//     pthread_t tid;
//     pthread_create(&tid, nullptr, threadRoutine, nullptr);
//
//     printf("main thread, pid is:%d\n",getpid());
//     // cout << "main thread, pid is:" << getpid() << endl;
//
//     // 使用函数 pthread_join 进程等待
//     void *retval;
//     pthread_join(tid, &retval);
//     cout << "main thread quit ..., ret: " << (long long int)retval << endl;
//     return 0;
// }