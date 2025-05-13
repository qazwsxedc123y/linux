// 使用signal函数自定义SIGINT信号的处理方式
// 自定义处理函数名称为“sigcb”, 在sigcb当中完成打印触发本次事件的信号值
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void sigcb(int sig)
{
    printf("caught a signal %d\n", sig);
}

int main()
{
    signal(2, sigcb);
    while(1)
    {
        printf(" waiting signal\n");
        sleep(1);
    }
    return 0;
}