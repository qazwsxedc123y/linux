// 单向通信，server为读端
// 引用相同的头文件，comm.hpp
#include "comm.hpp"
#include "log.hpp"
// 添加打印日志信息
// 同样用类进行封装
// 日志信息包括：日志时间，日志的等级，日志内容，日志的名称和行号
// 日志的等级：
// Iofo：常规消息
// Waring：报警信息
// Error：必要严重了，可能要立即处理
// Fatel：致命的
// Debug：调试

using namespace std;

// 作为读端要做的就是
// 1.创建管道
// 2.打开管道
// 3.作为读端进行读数据 -- 开始通信
// 4.关闭管道 删除管道文件unlink (参数为path路径)

int main()
{
    // 1.对于创建管道要用  mkfifo 函数 第一个参数为filenaem，第二个为 mode （umask）
    // 代码优化，将创建管道放到一个类里面，comm.hpp内
    // 这样代码就大大优化
    Init init;

    Log log;
    //log.Enable(Onefile);
    log.Enable(Classfile);
    

    // 2.打开管道文件  使用open  第二个参数为打开方式    
    int fd=open(FIFO_FILE,O_RDONLY); // 等待写入方打开之后，自己才会打开文件，向后执行， open 阻塞了！

    if (fd < 0)// 打开失败
    {
        log(Fatal, "error string: %s, error code: %d", strerror(errno), errno);
        exit(FIFO_OPEN_ERR);
    }

    cout << "server open file done" << endl;

    log(Info, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log(Warning, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log(Fatal, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log(Debug, "server open file done, error string: %s, error code: %d", strerror(errno), errno);


    // 3.开始通信
    while(true)
    {
        char buffer[1024] = {0};
        // 读取到buffer内，然后输出
        int x=read(fd,buffer,sizeof(buffer));// 向fd管道中读取，读入到buffer内
        if(x > 0)// 读取成功
        {
            buffer[x]= 0;
            cout << "client say# " << buffer << endl;
        }
        else if (x == 0)// 失败
        {
            log(Debug, "client quit, me too!, error string: %s, error code: %d", strerror(errno), errno);
            break;
        }
        else
        {
            break;
        }
    }


    // 4.关闭管道 删除管道文件unlink (参数为path路径)
    close(fd);
    // 同样删除有也放到类里面

    return 0;
}