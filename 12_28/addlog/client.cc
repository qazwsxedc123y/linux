// 单向通信，client为写端
// 引用相同的头文件，comm.hpp
#include <iostream>
#include "comm.hpp"

using namespace std;

// 作为写端只需要打开管道文件，然后向管道文件内写入，不写了就关闭close

int main()
{
    int fd = open(FIFO_FILE, O_WRONLY);
    if(fd < 0)
    {
        perror("open");
        exit(FIFO_OPEN_ERR);
    }
    cout << "client open file done" << endl;
    string line;
    while(true)
    {
        cout << "Please Enter@ ";
        getline(cin,line);
        write(fd, line.c_str(), line.size());
    }
    close(fd);
    return 0;
}