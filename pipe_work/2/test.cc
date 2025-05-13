#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;
// 使用sigprocmask函数阻塞2号信号和40号信号
// 阻塞2号信号和40号信号， 分别给进程发送5次2号信号和5次40号信号
void handler(int sig)
{
    printf("caught a signal %d\n", sig);
}

int main()
{
    printf("PID : %d\n", getpid());
    signal(2, handler);
    signal(40, handler);

    sigset_t set, oset;
    sigemptyset(&set);
    sigemptyset(&oset);

    sigaddset(&set, 2); // 2
    sigaddset(&set, 40); // 40

    sigprocmask(SIG_SETMASK, &set, &oset);// 阻塞信号
    sleep(50);
    return 0;
}