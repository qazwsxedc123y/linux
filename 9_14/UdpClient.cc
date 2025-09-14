#include "UdpClient.hpp"

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << " serverip serverport\n" << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }

    // 同样作为客户端，我们是先发信息，然后经过服务端处理后才会返回让客户端接收到
    // 同样要创建套接字
    // int sockfd_;     // 网路文件描述符
    // std::string ip_; // 任意 ip 地址  表示的是服务器的IP地址。具体来说，ip_ 是用来绑定服务器的网络接口的IP地址。
    // uint16_t port_;  // 表明服务器进程的端口号

    std::string server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);
    UdpClient* clt = new UdpClient(server_ip, server_port);
	clt->InitClient();
	clt->Start();

    return 0;
}