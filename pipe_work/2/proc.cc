// 使用sigaction函数自定义SIGINT信号的处理方式 ;// 2
// 自定义处理函数名称为“sigcb”, 在sigcb当中完成打印信号值
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void sigcb(int signo) 
{
    printf("caught signal: %d\n", signo);
}

int main()
{
    printf("PID: %d\n", getpid());
    struct sigaction act;
    act.sa_handler = sigcb;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(2, &act, nullptr);
    sleep(10);
    return 0;
}