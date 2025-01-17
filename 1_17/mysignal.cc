#include <iostream>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

// 必须得等待吗？？？必须得调用 wait吗？？
void handler(int signo)
{
    sleep(5);
    pid_t rid;
    while ((rid = waitpid(-1, nullptr, WNOHANG)) > 0)
    {
        cout << "I am proccess: " << getpid() << " catch a signo: " << signo << "child process quit: " << rid << endl;
    }
}

int main()
{
    // 当一个子进程退出时，它会给父进程发送信号 SIGCHLD（信号编号为 17）。这个信号的目的是通知父进程它的子进程已经退出（或停止）。
    signal(17, SIG_IGN); // SIG_DFL -> action -> IGN

    // srand(time(nullptr));
    // signal(17, handler);
    // 如果我们有10个子进程呢？？如果同时退出呢？
    // 如果退出一半呢？
    for (int i = 0; i < 10; i++)
    {
        pid_t id = fork();
        if (id == 0)
        {
            while (true)
            {
                cout << "I am child process: " << getpid() << ", ppid: " << getppid() << endl;
                sleep(5);
                break;
            }
            cout << "child quit!!!" << endl;
            exit(0);
        }
        // sleep(rand()%5+3);
        sleep(1);
    }
    // father
    while (true)
    {
        cout << "I am father process: " << getpid() << endl;
        sleep(1);
    }

    return 0;
}


// // g++ -o      -3// 最大  -0// 无优化
// volatile int flag = 0;

// void handler(int signo)
// {
//     cout << "catch a signal: " << signo << endl;
//     flag = 1;
// }

// int main()
// {
//     signal(2, handler);
//     // 在优化条件下， flag变量可能被直接优化到CPU内的寄存器中
//     while(!flag); // flag 0, !falg 真

//     cout << "process quit normal" << endl;
//     return 0;
// }



// // 问题1： pending位图，什么时候从1->0. 执行信号捕捉方法之前，先清0，在调用
// // 问题2： 信号被处理的时候，对应的信号也会被添加到block表中，防止信号捕捉被嵌套调用

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