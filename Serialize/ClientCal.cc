#include <iostream>
#include <string>
#include <ctime>
#include <cassert>
#include <unistd.h>
#include "Socket.hpp"
#include "Protocol.hpp"

static void Usage(const std::string &proc)
{
    std::cout << "\nUsage: " << proc << " port\n" << std::endl;
}

// ./clientcal ip port
int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string serverip = argv[1];
    uint16_t serverprot = std::stoi(argv[2]);

    Sock sockfd;
    sockfd.Socket();
    bool r = sockfd.Connect(serverip, serverprot);
    if(!r) return 1;

    // 对于客户端对数据的处理步骤：
    // 先构造变量对象
    // 后对数据序列化
    // 编码
    // 才可以发送

    // 对于接受数据
    // 先接受
    // 对其解码
    // 反序列化
    // 才可以答应获得的数据

    return 0;
}