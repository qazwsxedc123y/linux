#pragma once

#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include "Socket.hpp"

using namespace std;

static const uint16_t defaultport = 8080;
static const int fd_num_max = (sizeof(fd_set) * 8);
int defaultfd = -1;

class SelectServer
{
public:
    SelectServer(uint16_t port = defaultport) : _port(port)
    {
        for(int i = 0; i < fd_num_max; i++)
        {
            fd_array[i] = defaultfd;
        }
    }
    bool Init()
    {
        _listensock.Socket();
        _listensock.Bind(_port);
        _listensock.Listen();

        return true;
    }
    void Accepter()
    {
        std::string clientip;
        uint16_t clientport = 0;
        int sock = _listensock.Accept(&clientip, &clientport); // 会不会阻塞在这里？不会
        if(sock < 0) return ;
        lg(Info, "accept success, %s: %d, sock fd: %d", clientip.c_str(), clientport, sock);

        int pos = 1;
        for(; pos < fd_num_max; pos++)
        {
            if(fd_array[pos] != defaultfd) continue;
            else
                break;
        }
        if(pos == fd_num_max)
        {
            lg(Warning, "server is full, close %d now!", sock);
            close(sock);
        }
        else
        {
            fd_array[pos] = sock;
            PrintFd();
            // TODO
        }
    }
    void Recver(int fd, int pos)
    {
        char buffer[1024];
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
        if(n > 0)
        {
            buffer[n] = 0;
            cout << "get a messge: " << buffer << endl;
        }
        else if(n == 0)
        {
            lg(Info, "client quit, me too, close fd is : %d", fd);
            close(fd);
            fd_array[pos] = defaultfd; // 这里本质是从select中移除
        }
        else
        {
            lg(Warning, "recv error: fd is : %d", fd);
            close(fd);
            fd_array[pos] = defaultfd;
        }
    }
    void Dispatcher(fd_set &rfds)
    {
        for (int i = 0; i < fd_num_max; i++)
        {
            int fd = fd_array[i];
            if(fd == defaultfd) continue;

            if(FD_ISSET(fd, &rfds))
            {
                if(fd == _listensock.Fd())
                {
                    Accepter(); // 连接管理器
                }
                else
                {
                    Recver(fd, i);
                }
            }
        }
    }
    void Start()
    {
        int listensock = _listensock.Fd();
        fd_array[0] = listensock;
        for(;;)
        {
            fd_set rfds;
            FD_ZERO(&rfds);

            int maxfd = fd_array[0];
            for(int i = 0; i < fd_num_max; i++)
            {
                if(fd_array[i] == defaultfd) continue;
                FD_SET(fd_array[i], &rfds);
                if(maxfd < fd_array[i])
                {
                    maxfd = fd_array[i];
                    lg(Info, "max fd update, max fd is: %d", maxfd);
                }
            }
            // struct timeval timeout = {1, 0}; // 输入输出，可能要进行周期的重复设置
            struct timeval timeout = {0, 0};
            int n = select(maxfd + 1, &rfds, nullptr, nullptr, /*&timeout*/ nullptr);
            switch(n)
            {
                case 0:
                    cout << "time out, timeout : " << timeout.tv_sec << "." << timeout.tv_usec << endl;
                    break;
                case -1:
                    cerr << "select error" << endl;
                    break;
                default :
                    // 有事件就绪了，TODO
                    cout << "get a link!!!" << endl;
                    Dispatcher(rfds);
                    break;
            }
        }

    }
    void PrintFd()
    {
        cout << "online fd list: ";
        for (int i = 0; i < fd_num_max; i++)
        {
            if (fd_array[i] == defaultfd)
                continue;
            cout << fd_array[i] << " ";
        }
        cout << endl;
    }

    ~SelectServer()
    {
        _listensock.Close();
    }
private:
    Sock _listensock;
    uint16_t _port;
    int fd_array[fd_num_max];   // 数组, 用户维护的！
};