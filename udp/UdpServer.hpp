#pragma once
#include <unistd.h>
#include <iostream>
#include <string>
#include <strings.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include "Log.hpp"

//  函数类型别名

// using func_t = std::function<std::string(const std::string&)>;
typedef std::function<std::string(const std::string&)> func_t;

Log lg;

enum
{
    SOCKET_ERR = 1,
    BIND_ERR
};

uint16_t defaultport = 8080;
std::string defaultip = "0.0.0.0";
const int size = 1024;

class UdpServer
{
public:
    UdpServer(const uint16_t &port = defaultport, const std::string &ip = defaultip) : sockfd_(0), port_(port), ip_(ip), isrunning_(false)
    {}
    void Init()
    {
        // 1. 创建udp socket
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd_ < 0)
        {
            lg(Fatal, "socket create error, sockfd: %d", sockfd_);
            exit(SOCKET_ERR);
        }
        // 2. bind socket
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port_); // htons用于将 16位整数（端口号） 从 主机字节序转换为 网络字节序 (大小端)
        // inet_addr(ip_.c_str()) 是一个用于将 点分十进制格式的IP地址字符串（如 "192.168.1.1"）
        // 转换为 32位无符号整数（uint32_t） 的函数，
        // 并且这个整数是 网络字节序（大端序） 的。
        local.sin_addr.s_addr = inet_addr(ip_.c_str());

        if(bind(sockfd_, (const struct sockaddr *)&local, sizeof(local)) < 0)
        {
            lg(Fatal, "bind error, errno: %d, err string: %s", errno, strerror(errno));
            exit(BIND_ERR);
        }
        lg(Info, "bind success, errno: %d, err string: %s", errno, strerror(errno));
    }
    void Run(func_t func) // 对代码进行分层
    {
        isrunning_ = true;
        char inbuffer[size];
        
        while(isrunning_)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            // std::cout << "4" << std::endl;
            // 接收数据
            ssize_t n = recvfrom(sockfd_, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr*)&client, &len);
            if(n < 0)
            {
                lg(Warning, "recvfrom error, errno: %d, err string: %s", errno, strerror(errno));
                continue;
            }
            inbuffer[n] = 0;
            std::string info = inbuffer;
            // 调用回调函数
            std::string echo_string = func(info);
            // std::cout << "3" << std::endl;
            // echo_string.c_str()：要发送的数据（std::string 转换为 C 风格字符串）。
            sendto(sockfd_, echo_string.c_str(),  echo_string.size(), 0, (struct sockaddr*)&client, len);
        }
    }
    ~UdpServer()
    {
        if(sockfd_>0) close(sockfd_);
    }
private:
    int sockfd_;     // 网路文件描述符
    std::string ip_; // 任意 ip 地址  表示的是服务器的IP地址。具体来说，ip_ 是用来绑定服务器的网络接口的IP地址。
    uint16_t port_;  // 表明服务器进程的端口号
    bool isrunning_;
};

// ip_ 的作用：ip_ 是服务器绑定的IP地址。服务器可以通过这个IP地址来监听来自客户端的请求。
// 默认值：在代码中，ip_ 的默认值是 "0.0.0.0"，这是一个特殊的IP地址，表示服务器将绑定到所有的网络接口上。
// 也就是说，服务器会监听所有可用的网络接口（包括本地回环接口 127.0.0.1 和所有物理网卡的IP地址）。
// 绑定到特定IP：如果你希望服务器只监听特定的网络接口（例如，只监听某个特定的网卡IP地址），你可以将 ip_
// 设置为该网卡的IP地址。例如，ip_ = "192.168.1.100"，这样服务器只会监听 192.168.1.100 这个IP地址上的请求。