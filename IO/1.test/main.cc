#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <fcntl.h>

using namespace std;

void SetNonBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if(fl < 0)
    {
        perror("fcntl");
        return ;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
    cout << " set " << fd << " nonblock done" << endl;
}

int main()
{
    char buffer[1024];
    SetNonBlock(0);
    sleep(1);
    while(true)
    {
        ssize_t n = read(0, buffer, sizeof(buffer) - 1);
        if(n > 0)
        {
            buffer[n - 1] = 0;
            cout << "ehco : " << buffer << endl;
        }
        else if(n == 0)
        {
            cout << "read done" << endl;
            break;
        }
        else
        {
            // 1. 设置成为非阻塞，如果底层fd数据没有就绪，recv/read/write/send, 返回值会以出错的形式返回
            // 2. a. 真的出错 b. 底层没有就绪
            // 3. 我怎么区分呢？通过errno区分！！！
            if(errno == EWOULDBLOCK)
            {
                cout << "0 fd data not ready, try again!" << endl;
                sleep(1);
            }
            else
            {
                cout << "read error , n = " << n << "error code : "
                    << errno << ", error is" << strerror(errno) << endl;
            }
        }
    }
    return 0;
}