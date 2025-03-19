#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Log.hpp"


// 先创建 sockfd 文件描述符
// 绑定端口号 bind
// 设定监听模式 listen
// 接受请求
// 建立连接
// 最后 最好要关闭sockfd  close


// 定义了操作系统内核中等待处理的连接请求队列的最大长度   --》  listen函数相关
const int backlog = 10;

enum
{
    SockErr = 2,
    BindErr,
    ListenErr,
};

class Sock
{
public:
    Sock()
    {

    }
    ~Sock()
    {

    }
public:
    void Socket()
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd_ < 0)
        {
            lg(Fatal, "sockfd error, %s, %d", strerror(errno), errno);
            exit(SockErr);
        }
    }
    void Bind(uint16_t port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(port);

        if(bind(sockfd_, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            lg(Fatal, "bind error, %s, %d", strerror(errno), errno);
            exit(BindErr);
        }
    }
    void Listen()
    {
        if (listen(sockfd_, backlog) < 0)
        {
            lg(Fatal, "listen error, %s, %d", strerror(errno), errno);
            exit(ListenErr);
        }
    }
    int Accept(std::string *clientip, uint16_t *clientport)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int newfd = accept(sockfd_, (struct sockaddr*)&peer, &len);
        if(newfd < 0)
        {
            lg(Warning, "accept error, %s: %d", strerror(errno), errno);
            return -1;
        }

        char ipstr[64];
        inet_ntop(AF_INET, &peer.sin_addr, ipstr, sizeof(ipstr));
        *clientip = ipstr;
        *clientport = ntohs(peer.sin_port);

        return newfd;
    }
    bool Connect(const std::string &ip, const uint16_t &port)
    {
        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        peer.sin_family = AF_INET;
        peer.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &(peer.sin_addr));

        int n = connect(sockfd_, (struct sockaddr*)&peer, sizeof(peer));
        if(n == -1) 
        {
            std::cerr << "connect to " << ip << ":" << port << " error" << std::endl;
            return false;
        }
        return true;
    }
    void Close()
    {
        close(sockfd_);
    }
    int Fd()
    {
        return sockfd_;
    }
    
private:
    int sockfd_;
};