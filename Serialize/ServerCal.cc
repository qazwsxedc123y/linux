#include "TcpServer.hpp"
#include "ServerCal.hpp"
#include <unistd.h>
// #include "Daemon.hpp"

static void Usage(const std::string &proc)
{
    std::cout << "\nUsage: " << proc << " port\n" << std::endl;
}

// ./servercal 8080 (端口号)
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }
    uint16_t port = std::stoi(argv[1]);

    // 不会写，但是能懂一点，这是设计一个回调函数，将cal中的函数Calculator绑定为回调函数
    ServerCal cal;
    TcpServer *tsvr = new TcpServer(port, std::bind(&ServerCal::Calculator, &cal, std::placeholders::_1));

    tsvr->InitServer();
    tsvr->Start();

    return 0;
}