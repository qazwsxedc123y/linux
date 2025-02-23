#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Terminal.hpp"

using namespace std;
const int SIZE = 1024;

struct ThreadData
{
    struct sockaddr_in server;
    int sockfd_;
    std::string serverip_;
};

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << " serverip serverport\n" << std::endl;
}

void *recv_message(void *args)
{
    OpenTerminal();
    ThreadData *td = static_cast<ThreadData *>(args);
    char buffer[SIZE];
    while(true)
    {
        memset(buffer, 0, sizeof(buffer));

        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);

        ssize_t s = recvfrom(td->sockfd_, buffer, 1023, 0, (struct sockaddr *)&temp, &len);

        if(s > 0)
        {
            buffer[s] = 0;
            cerr << buffer << endl;
        }
    }
}
void *send_message(void *args)
{
    ThreadData *td = static_cast<ThreadData *>(args);
    string message;
    socklen_t len = sizeof(td->server);

    // std::string welcome = td->serverip_;
    // welcome += " comming...";
    // sendto(td->sockfd_, message.c_str(), message.size(), 0, (struct sockaddr *)&(td->server), len);

    while(true)
    {
        cout << "Please Enter@ ";
        getline(cin, message);
        sendto(td->sockfd_, message.c_str(), message.size(), 0, (struct sockaddr *)&(td->server), len);
    }
}

// 多线程
// 将自己发的信息，与别人发的信息分割开，这样便于观察，就类似于聊天框似的
// ./udpclient serverip serverport
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

    string serverip;
    uint16_t serverport = std::stoi(argv[2]);
    serverip = argv[1];

    struct ThreadData td;

    bzero(&td.server, sizeof(td.server));
    // 保存ip
    td.server.sin_addr.s_addr = inet_addr(serverip.c_str());
    td.server.sin_family = AF_INET;
    td.server.sin_port = htons(serverport);
    socklen_t len = sizeof(td.server);
    
    td.sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if(td.sockfd_ < 0)
    {
        cout << "socker error" << endl;
        return 1;
    }
    td.serverip_ = serverip;

    pthread_t recvr, sender;
    pthread_create(&recvr, nullptr, recv_message, &td);
    pthread_create(&sender, nullptr, send_message, &td);

    pthread_join(recvr, nullptr);
    pthread_join(sender, nullptr);

    close(td.sockfd_);
    return 0;
}