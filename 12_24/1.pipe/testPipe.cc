#include <iostream>
#include <string>
#include <cstring>// 等同于c语言中的string.h
#include <cstdlib> //stdlib.h
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

#define N 2
#define NUM 1024

void Writer(int wfd)
{
    // 先写入buffer内，后写入管道内

    string s = "hello, I am child";
    pid_t self=getpid();
    int number=0;

    char buffer[NUM];
    while(true)
    {
        sleep(1);
        // 构建发送字符串
        buffer[0] = 0; // 字符串清空, 只是为了提醒阅读代码的人，我把这个数组当做字符串了
        snprintf(buffer, sizeof(buffer), "%s-%d-%d", s.c_str(), self, number++);

        // 发送/写入给父进程, system call
        write(wfd,buffer, strlen(buffer));
    }
}

void Reader(int rfd)
{
    // 读取到buffer里面

    char buffer[NUM];
    int cnt = 0;
    while(true)
    {
        buffer[0] = 0; 
        // system call
        size_t n = read(rfd, buffer, sizeof(buffer)); //sizeof != strlen
        if(n > 0)
        {
            buffer[n] = 0; // 0 == '\0'
            cout << "father get a message[" << getpid() << "]# " << buffer << endl;
        }
        else if(n == 0) 
        {
            printf("father read file done!\n");
            break;
        }
        else break;
        cnt++;
        if(cnt>5) break;
    }
 
}

int main()
{
    // pipe函数为输出型参数
    int pipefd[N]={0};
    int n=pipe(pipefd);
    //fd[0]  读端   fd[1]  写端
    if(n<0) return 1;// pipe调用失败
    // fork函数  使子为fd[1]    父为fd[0]
    //     child -> w, father->r
    pid_t id=fork();
    // 子进程返回0， 父进程返回子进程的pid
    if (id < 0)  return 2;// fork失败
    if (id == 0)
    {
        // child
        close(pipefd[0]); // 子进程关闭读端

        // 子进程进行向管道进行写
        // IPC code
        Writer(pipefd[1]);

        // 关闭，临走操作
        close(pipefd[1]);
        exit(0);
    }

    // father
    close(pipefd[1]);

    Reader(pipefd[0]); // 读取5s
    close(pipefd[0]);

    // 避免僵尸进程
    int status = 0;
    pid_t rid = waitpid(id, &status, 0);
    if(rid < 0) return 3;

    cout << "wait child success: " << rid << " exit code: " << ((status>>8)&0xFF) << " exit signal: " << (status&0x7F) << endl;

    sleep(5);

    cout << "father quit" << endl;

    return 0;
}