#include <stdio.h>
#include <signal.h>
#include <unistd.h>





// 使用函数调用产生信号 abort

// void myhandler(int signo)
// {
//     printf("process get a signal: %d\n", signo);
// }

// int main()
// {
//     signal(6, myhandler);
//     while(1){
//         printf("I am a process, I am waiting signal, pid : %d\n",getpid());
//         sleep(1);
//         abort();
//     }
//     return 0;
// }


// 使用函数调用产生信号 raise

// void myhandler(int signo)
// {
//     printf("process get a signal: %d\n", signo);
// }

// int main()
// {
//     signal(3, myhandler);
//     while(1){
//         printf("I am a process, I am waiting signal, pid : %d\n",getpid());
//         sleep(1);
//         raise(3);
//     }
//     return 0;
// }


// 使用函数调用产生信号 kill

// void myhandler(int signo)
// {
//     printf("process get a signal: %d\n", signo);
// }

// int main()
// {
//     signal(3, myhandler);
//     while(1){
//         printf("I am a process, I am waiting signal, pid : %d\n",getpid());
//         sleep(1);
//         kill(getpid(), 3);
//     }
//     return 0;
// }