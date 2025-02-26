#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void Usage(const std::string &proc)
{
    std::cout << "\n\rUsage: " << proc << " serverip serverport\n"
              << std::endl;
}

// ./tcpclient serverip serverport
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    inet_pton(AF_INET, serverip.c_str(), &(server.sin_addr));

    while (true)
    {
        int sockfd;
        int cnt = 5;
        int isreconnect = false;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            std::cerr << "socket error" << std::endl;
            return 1;
        }
        do
        {
            // tcp客户端要不要bind？1 要不要显示的bind？0 系统进行bind，随机端口
            // 客户端发起connect的时候，进行自动随机bind
            int n = connect(sockfd, (struct sockaddr *)&server, sizeof(server));
            if (n < 0)
            {
                isreconnect =true;
                std::cerr << "connect error..., reconnect: " << std::endl;
                cnt--;
                sleep(2);
            }
            else // 重连成功
            {
                break;
            }
        } while (cnt && isreconnect);

        if(cnt == 0)
        {
            std::cerr << "use offline..." << std::endl;
            break;
        }

        std::string message;
        // 先发
        std::cout << "Please Enter# ";
        std::getline(std::cin, message);

        int n = write(sockfd, message.c_str(), sizeof(message));
        if (n < 0)
        {
            std::cerr << "write error..." << std::endl;
        }
        // 后读server返回的信息
        char inbuffer[4096];
        n = read(sockfd, inbuffer, sizeof(inbuffer));
        if (n > 0)
        {
            inbuffer[n] = 0;
            std::cout << inbuffer << std::endl;
        }
        close(sockfd);
    }
    return 0;
}