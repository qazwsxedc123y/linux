#include "UdpServer.hpp"

int main()
{
    UdpServer* svr = new UdpServer();
    svr->InitServer();
    
    svr->Start();
    svr->~UdpServer();
    return 0;
}