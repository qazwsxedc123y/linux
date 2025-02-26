#pragma once

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const std::string nullfile = "/dev/null";

void Daemon(const std::string &cwd = "")
{
    // 忽略异常信号
    // SIGCLD（或 SIGCHLD）是子进程终止或停止时发送给父进程的信号。通过忽略这个信号，父进程不会因为子进程的终止而收到通知。
    // SIGPIPE 是当进程尝试向一个已经关闭的管道写入数据时发送的信号。忽略这个信号可以防止进程因为写入失败而意外终止。
    // SIGSTOP 是停止进程执行的信号。忽略这个信号可以防止进程被意外停止。
    // 这些信号的忽略是为了确保守护进程在后台运行时不会因为某些信号而意外终止或停止。
    signal(SIGCLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    // 创建独立的会话
    if(fork() > 0) exit(0);
    // setsid() 创建一个新的会话，并使当前进程成为该会话的领头进程。这样，当前进程就不再与任何终端关联，成为一个独立的守护进程。
    setsid();

    // 更改工作目录
    // 如果调用 Daemon 函数时传入了工作目录 cwd，则将当前进程的工作目录更改为 cwd。这是为了防止守护进程占用某个挂载点，导致该挂载点无法卸载。
    if(!cwd.empty())
        chdir(cwd.c_str());
    
    // 重定向标准输入、输出和错误
    // open(nullfile.c_str(), O_RDWR) 打开 /dev/null 文件，该文件是一个特殊的设备文件，读取它会立即返回 EOF，写入它的数据会被丢弃。
    int fd = open(nullfile.c_str(), O_RDWR);
    if(fd > 0)
    {
        // new 为 old 的一份拷贝
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
        // close(fd) 关闭之前打开的文件描述符，避免资源泄漏。
        close(fd);
    }
}