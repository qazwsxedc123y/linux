#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

volatile int flag = 0;

void handler(int signo)
{
	printf("get a signal:%d\n", signo);
	flag = 1;
}
int main()
{
	signal(2, handler);
	while (!flag);
    cout << flag << endl;
	printf("process quit normal\n");
	return 0;
}


// void PrintPending()
// {
//     sigset_t set;
//     sigpending(&set);

//     for (int signo = 1; signo <= 31; signo++)
//     {
//         if (sigismember(&set, signo))
//             cout << "1";
//         else
//             cout << "0";
//     }
//     cout << "\n";
// }

// void handler(int signo)
// {
//     cout << "catch a signal, signal number : " << signo << endl;
//     while (true)
//     {
//         PrintPending();
//         sleep(1);
//     }
// }

// int main()
// {
//     // act: 用于设置新的信号处理动作。
//     // oact: 用于保存原来的信号处理动作。
//     struct sigaction act, oact;


//     // 将 act 和 oact 的所有字段初始化为 0，以确保没有意外的垃圾数据。
//     memset(&act, 0, sizeof(act));
//     memset(&oact, 0, sizeof(oact));


//     // 初始化 act.sa_mask，表示在信号处理期间不屏蔽任何信号。
//     sigemptyset(&act.sa_mask);


//     // 将信号 1、3 和 4 添加到 act.sa_mask 中。
//     // 这些信号在处理信号 2 的过程中会被阻塞，避免它们干扰当前信号的处理。
//     sigaddset(&act.sa_mask, 1);// 设置block表
//     sigaddset(&act.sa_mask, 3);
//     sigaddset(&act.sa_mask, 4);

//     // 指定处理信号 2 的动作。
//     act.sa_handler = handler; // SIG_IGN SIG_DFL  设置捕捉动作函数
//     sigaction(2, &act, &oact);

//     while (true)
//     {
//         cout << "I am a process: " << getpid() << endl;
//         sleep(1);
//     }
//     return 0;
// }