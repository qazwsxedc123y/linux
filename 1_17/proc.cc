#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

void PrintPending(sigset_t & pending)
{
    for(int signo=31;signo>=1;signo--)
    {
        if(sigismember(&pending, signo)) // 判断是否存在，存在返回1， 不存在返回0
        {
            cout<<"1";
        }
        else
        {
            cout << "0";
        }
    }
    cout << "\n\n";
}

void handler(int signo)
{
    cout << "catch a signo: " << signo << endl;
}


int main()
{

    // 0. 对2号信号进行自定义捕捉
    signal(2, handler);


    // 1. 先对2号信号进行屏蔽 --- 数据预备
    sigset_t bset, oset;
    sigemptyset(&bset);// 清空
    sigemptyset(&oset);// 清空


    // 1.2 调用系统调用，将数据设置进内核

    // sigaddset(&bset,2); // 仅仅对二号信号进行屏蔽
    // sigprocmask(SIG_SETMASK, &bset, &oset);


    // 对于block表进行全屏蔽

    for(int i=1;i<=31;i++)
    {
        sigaddset(&bset, i); // 屏蔽了所有信号吗？？？   添加信号
    }
    sigprocmask(SIG_SETMASK, &bset, &oset);// 修改block表   修改为bset，然后原来的保存到oset，这一步是为了恢复


    // 2. 重复打印当前进程的pending 0000000000000000000000000
    sigset_t pending;
    int cnt=0; // 设置第20s解除屏蔽
    while(true)
    {
        // 获取 pending 表
        int n = sigpending(&pending);
        if(n < 0) continue;
        // 打印
        PrintPending(pending);
        sleep(1);
        cnt++;
        // 2.3 解除阻塞
        if(cnt == 20)
        {
            cout << "unblock 2 signo" << endl;
            sigprocmask(SIG_SETMASK, &oset, nullptr); // 我们已经把2好信号屏蔽了吗？ok
        }
    }
    
    // 3 发送2号 0000000000000000000000010
    return 0;
}