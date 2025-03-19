#include <iostream>
#include <string>
#include <ctime>
#include <cassert>
#include <unistd.h>
#include "Socket.hpp"
#include "Protocol.hpp"

static void Usage(const std::string &proc)
{
    std::cout << "\nUsage: " << proc << " port\n"
              << std::endl;
}

// ./clientcal ip port
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string serverip = argv[1];
    uint16_t serverprot = std::stoi(argv[2]);

    Sock sockfd;
    sockfd.Socket();
    bool r = sockfd.Connect(serverip, serverprot);
    if (!r)
        return 1;

    srand(time(nullptr) ^ getpid());
    int cnt = 1;
    const std::string opers = "+-*/%=&#^";

    std::string inbuffer_stream;
    while (cnt <= 10)
    {
        // 对于客户端对数据的处理步骤：
        // 先构造变量对象
        // 后对数据序列化
        // 编码
        // 才可以发送
        std::cout << "===============第" << cnt << "次测试....., " << "===============" << std::endl;
        int x = rand() % 100;
        usleep(1234);
        int y = rand() % 100;
        usleep(1234);
        char oper = opers[rand() % opers.size()];
        Request req(x, y, oper);
        req.DebugPrint();

        std::string package;
        req.Serialize(&package);
        package = Encode(package);

        write(sockfd.Fd(), package.c_str(), sizeof(package));

        // 对于接受数据
        // 先接受
        // 对其解码
        // 反序列化
        // 才可以答应获得的数据

        char inbuff[128];
        size_t n = read(sockfd.Fd(), inbuff, sizeof(inbuff)); // 我们也无法保证我们能读到一个完整的报文
        if (n > 0)
        {
            inbuff[n] = 0;
            inbuffer_stream += inbuff;
            std::cout << inbuffer_stream << std::endl;
            std::string content;
            bool r = Decode(inbuffer_stream, &content);
            assert(r);

            Response resp;
            r = resp.Deserialize(content);
            assert(r);

            resp.DebugPrint();
        }

        std::cout << "=================================================" << std::endl;
        sleep(1);

        cnt++;
    }

    sockfd.Close();

    return 0;
}