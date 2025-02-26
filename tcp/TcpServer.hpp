#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <signal.h>
#include "Log.hpp"
#include "ThreadPool.hpp"
#include "Task.hpp"

const int defaultfd = -1;
const std::string defaultip = "0.0.0.0"; // ip
const int backlog = 10;                  // 但是一般不要设置的太大
const int size = 4096;
extern Log lg;

enum
{
    UsageError = 1,
    SocketError,
    BindError,
    ListenError,
};

class TcpServer;

class ThreadData
{
public:
    ThreadData(int fd, const std::string &ip, const uint16_t &p, TcpServer *t): sockfd(fd), clientip(ip), clientport(p), tsvr(t)
    {}
public:
    int sockfd;
    std::string clientip;
    uint16_t clientport;
    TcpServer *tsvr;
};

class TcpServer
{
public:
    TcpServer(const uint16_t &port, const std::string &ip = defaultip) : listensock_(defaultfd), port_(port), ip_(ip)
    {
    }
    void InitServer()
    {
        // 创建监听套接字（socket）。
        listensock_ = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM 表示面向连接的、可靠的、基于字节流的通信方式，通常用于 TCP 协议。
        if (listensock_ < 0)
        {
            lg(Fatal, "create socket, errno: %d, errstring: %s", errno, strerror(errno));
            exit(SocketError);
        }
        lg(Info, "create socket success, listensock_: %d", listensock_);

        // std::cout << "listensock_ : " << listensock_ <<std::endl;

        // 设置套接字选项（setsockopt），允许地址和端口重用。
        // 目的是允许服务器在重启后立即重新使用相同的地址和端口。
        int opt = 1;
        setsockopt(listensock_, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt)); // 防止偶发性的服务器无法进行立即重启(tcp协议的时候再说)

        // 绑定套接字到指定的IP地址和端口（bind）。
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port_);
        inet_aton(ip_.c_str(), &(local.sin_addr)); // 比较新
        // local.sin_addr.s_addr = inet_addr(ip_.c_str()); 当然可以，但是这个函数比较旧
        if (bind(listensock_, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            lg(Fatal, "bind socket error, errno: %d, errstring: %s", errno, strerror(errno));
            exit(BindError);
        }
        lg(Info, "bind socket success, listensock_: %d", listensock_);

        // 开始监听连接请求（listen）。
        // 将套接字设置为监听状态，以便接受客户端的连接请求。
        // backlog监听队列的最大长度，表示同时等待处理的连接请求的最大数量。
        // 当多个客户端同时尝试连接服务器时，未处理的连接请求会被放入监听队列中。通常设置为一个较小的值（如 10），但具体值取决于服务器的负载和需求。
        // 监听队列分为两部分：
        // 未完成连接队列：存放尚未完成三次握手的连接请求。
        // 已完成连接队列：存放已经完成三次握手的连接请求，等待服务器调用 accept() 接受连接。
        if (listen(listensock_, backlog) < 0)
        {
            lg(Fatal, "listen socket error, errno: %d, errstring: %s", errno, strerror(errno));
            exit(ListenError);
        }
        lg(Info, "listen socket success, listensock_: %d", listensock_);
    }

    static void *Routine(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);
        td->tsvr->Service(td->sockfd, td->clientip, td->clientport);//???
        delete td;
        return nullptr;
    }

    void Start()
    {
        // 将服务器进程设置为守护进程（Daemon）。

        // 启动线程池（ThreadPool<Task>::GetInstance()->Start()）。

        ThreadPool<Task>::GetInstance()->Start();
        lg(Info, "tcpServer is running...");
        for(;;)
        {
            // 1. 获取新连接
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int sockfd = accept(listensock_, (struct sockaddr *)&client, &len); // 返回一个新的套接字文件描述符（非负整数），用于与客户端通信。
            if(sockfd < 0)
            {
                lg(Fatal, "accept error, errno: %d, errstring: %s", errno, strerror(errno));
                continue;
            }
            uint16_t clientport = ntohs(client.sin_port);
            char clientip[32];
            inet_ntop(AF_INET, &(client.sin_addr), clientip, sizeof(clientip));

            // 2. 根据新连接来进行通信
            lg(Info, "get a new link..., sockfd: %d, client ip: %s, client port: %d", sockfd, clientip, clientport);
            // // version 1 -- 单进程版
            // // std::cout << "sockfd : " << sockfd <<std::endl; // 4
            // Service(sockfd, clientip, clientport);
            // close(sockfd);

            // version 2 -- 多进程版
            // 但是这样，会阻塞等待子进程。还是跟单进程同样效果
            // 同一时间只允许单个用户使用
            // pid_t id = fork();
            // // 让子进程干活
            // if(id == 0)
            // {
            //     // child
            //     close(listensock_);
            //     Service(sockfd, clientip, clientport);
            //     close(sockfd);
            //     exit(0);
            // }
            // close(sockfd);
            // // father
            // pid_t rid = waitpid(id, nullptr, 0);
            // (void)rid;

            // 优化版

            // pid_t id = fork();
            // // 让子进程干活
            // if(id == 0)
            // {
            //     // child
            //     close(listensock_);
            //     if(fork() > 0) exit(0);
            //     Service(sockfd, clientip, clientport); //孙子进程， system 领养
            //     close(sockfd);
            //     exit(0);
            // }
            // close(sockfd);
            // // father
            // pid_t rid = waitpid(id, nullptr, 0);
            // (void)rid;

            // version 3 -- 多线程版本
            // ThreadData *td = new ThreadData(sockfd, clientip, clientport, this);
            // pthread_t tid;
            // pthread_create(&tid, nullptr, Routine, td);

            // version 4 --- 线程池版本
            Task t(sockfd, clientip, clientport);
            ThreadPool<Task>::GetInstance()->Push(t);
        }

        // 进入主循环，不断接受新的客户端连接（accept）。
        
        // 对于每个新连接，创建一个Task对象并将其推入线程池中，由线程池中的线程处理客户端的请求。
    }
    void Service(int sockfd, const std::string &clientip, const uint16_t &clientport)
    {
        char buffer[size];
        while(true)
        {
            ssize_t n = read(sockfd, buffer, sizeof(buffer));
            if(n > 0)
            {
                buffer[n] = 0;
                std::cout << "client say# " << buffer << std::endl;
                std::string echo_string = "tcpserver echo# ";
                echo_string += buffer;

                write(sockfd, echo_string.c_str(), echo_string.size());
            }
            else if (n == 0)
            {
                lg(Info, "%s:%d quit, server close sockfd: %d", clientip.c_str(), clientport, sockfd);
                break;
            }
            else
            {
                lg(Warning, "read error, sockfd: %d, client ip: %s, client port: %d", sockfd, clientip.c_str(), clientport);
                break;
            }
        }
    }

    ~TcpServer()
    {}
private:
    int listensock_;
    uint16_t port_;
    std::string ip_;
};