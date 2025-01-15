#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

// 由软件发送信号  --  闹钟
// 返回值为上一次闹钟的剩余时间
// 如果返回值0，则取消以前定的闹钟
// 反之不取消

void myhandler(int signo)
{
    cout << "...get a sig, number: " << signo <<endl; //我什么都没干，我只是打印了消息
    int n = alarm(5);
    cout << "剩余时间：" << n << endl;
}

void handler(int signo)
{
    cout << "...get a sig, number: " << signo <<endl; //我什么都没干，我只是打印了消息
}



int main()
{
    // //SIGALRM  14
    // //signal(SIGALRM, myhandler);

    // int n = alarm(10);
    // cout << "剩余时间：" << n << endl;

    // // 如果参数设置为0，则取消以前定的闹钟
    // int m = alarm(0);
    // cout << "剩余时间：" << n << endl;



    // alarm() 只能同时存在一个有效闹钟，每次调用都会取消之前的设置并返回之前闹钟的剩余时间。
    //新的闹钟完全覆盖了之前的设置。

    // signal(SIGALRM, handler);
    // sleep(1);
    // int n = alarm(10);
    // sleep(2);
    // alarm(3);

    // while(1)
    // {
    //     cout << "proc is running..., pid: " << getpid() << endl;
    //     sleep(1);
    // }
    return 0;
}