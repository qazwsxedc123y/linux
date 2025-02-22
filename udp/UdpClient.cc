#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
const int SIZE = 1024;

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << " serverip serverport\n" << std::endl;
}

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

    std::string serverip;
    uint16_t serverport = std::stoi(argv[2]);
    serverip = argv[1];

    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    // 保存ip
    server.sin_addr.s_addr = inet_addr(serverip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    socklen_t len = sizeof(server);
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        cout << "socker error" << endl;
        return 1;
    }
    // client 要bind吗？要！只不过不需要用户显示的bind！一般有OS自由随机选择！
    // 一个端口号只能被一个进程bind，对server是如此，对于client，也是如此！
    // 其实client的port是多少，其实不重要，只要能保证主机上的唯一性就可以！
    // 系统什么时候给我bind呢？首次发送数据的时候

    string message;
    char buffer[SIZE];
    while(true)
    {
        cout << "Please Enter@ ";
        getline(cin, message);
        // 1. 数据 2. 给谁发
        // 发送
        sendto(sockfd, message.c_str(), sizeof(message), 0, (struct sockaddr *)&server, len);
        // 接受对方处理后的信息
        struct sockaddr_in temp;
        socklen_t len1 = sizeof(temp);

        ssize_t n = recvfrom(sockfd,buffer, SIZE - 1, 0, (struct sockaddr *)&temp, &len1);
        if(n > 0)
        {
            buffer[n] = 0;
            cout << buffer << endl;
        }
    }

    close(sockfd);
    return 0;
}