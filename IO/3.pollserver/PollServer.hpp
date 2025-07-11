#pragma once

#include <iostream>
#include <poll.h>
#include <sys/time.h>
#include "Socket.hpp"

using namespace std;

static const uint16_t defaultport = 8080;
static const int fd_num_max = 64;
int defaultfd = -1;
int non_event = 0;

class PollServer
{
public:
    PollServer(uint16_t port = defaultport) : _port(port)
    {
        for(int i = 0; i < fd_num_max; i++)
        {
            _event_fds[i].fd = defaultfd;
            _event_fds[i].events = non_event;
            _event_fds[i].revents = non_event;
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
            if(_event_fds[pos].fd != defaultfd) 
                continue;
            else
                break;
        }
        if(pos == fd_num_max)
        {
            cout << "pos : " << pos << endl;
            lg(Warning, "server is full, close %d now!", sock);
            close(sock);
            // 扩容
        }
        else
        {
            _event_fds[pos].fd = sock;
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
            _event_fds[pos].fd = defaultfd; // 这里本质是从select中移除
        }
        else
        {
            lg(Warning, "recv error: fd is : %d", fd);
            close(fd);
           _event_fds[pos].fd = defaultfd;
        }
    }
    void Dispatcher()
    {
        for (int i = 0; i < fd_num_max; i++)
        {
            int fd = _event_fds[i].fd;
            if(fd == defaultfd) continue;

            if(_event_fds[i].revents & POLLIN)
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
        _event_fds[0].fd = _listensock.Fd();
        _event_fds[0].events = POLLIN;
        int timeout = 3000; // 3s
        for(;;)
        {
            int n = poll(_event_fds, fd_num_max, timeout);
            switch(n)
            {
                case 0:
                    cout << "time out... " << endl;
                    break;
                case -1:
                    cerr << "poll error" << endl;
                    break;
                default :
                    // 有事件就绪了，TODO
                    cout << "get a new link!!!!!" << endl;
                    Dispatcher(); // 就绪的事件和fd你怎么知道只有一个呢？？？
                    break;
            }
        }
    }
    void PrintFd()
    {
        cout << "online fd list: ";
        for (int i = 0; i < fd_num_max; i++)
        {
            if (_event_fds[i].fd == defaultfd)
                continue;
            cout << _event_fds[i].fd << " ";
        }
        cout << endl;
    }

    ~PollServer()
    {
        _listensock.Close();
    }
private:
    Sock _listensock;
    uint16_t _port;
    struct pollfd _event_fds[fd_num_max]; // 数组, 用户维护的！
};