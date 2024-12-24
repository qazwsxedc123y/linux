#include "Task.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
using namespace std;

// 创建10个管道
const int processnum = 10;
vector<task_t> tasks;


// 先描述
class channel
{
public:
    channel(int cmdfd, int slaverid, const string& processname)
        :_cmdfd(cmdfd), _slaverid(slaverid), _processname(processname)
    {}
public:
    int _cmdfd;               // 发送任务的文件描述符
    pid_t _slaverid;          // 子进程的PID
    string _processname;      // 子进程的名字 -- 方便我们打印日志
    // 子进程为读
    // 父进程为写
};

// // 子进程的执行任务  读
// void slaver(int rfd)
// {
//     // 设计成非阻塞轮询
//     // while循环
//     while(true)
//     {
//         cout<< getpid() << "--" << "read fd is : "<<rfd<<endl;
//         // 读取到
//         sleep(1000);
//     }
// }

// 子进程的执行任务  读
void slaver()// 改为标准输入从管道中读取
{
    // 设计成非阻塞轮询
    // while循环
    while(true)
    {
        // 设定为按四字节单位读取
        int cmdcode=0;
        int n=read(0, &cmdcode, sizeof(int)); // 如果父进程不给子进程发送数据呢？？阻塞等待！
        if(n == sizeof(int))// 读取成功
        {
            // 执行cmdcode对应的任务列表
            cout<< "slaver say : get a command: " <<getpid()<<"--"<<"cmdcode : "<<cmdcode<<endl;
            if(cmdcode>=0 && cmdcode<tasks.size()) tasks[cmdcode]();
        }
        if(n == 0) break; // 因为写端关闭，读端读到了末尾。slaver就会调完，就会执行下面。直到跑完代码，自己关闭（由操作系统回收）
    }
}

// 输入：const &
// 输出：*
// 输入输出：&
void InitProcessPool(vector<channel> *channels)
{
    for(int i=0;i<processnum;i++)
    {
        // 父 w  子 r
        // 先建立管道
        int pipefd[2]={0};// 临时空间
        int n=pipe(pipefd);
        assert(!n); // 检查报错是否pipe失败
        (void)n;
        pid_t id = fork();
        if(id==0)
        {
            // child
            close(pipefd[1]);           // 关w端
            dup2(pipefd[0],0);
            slaver();
            //slaver(pipefd[0]);          // 子进程执行相应的任务
            cout << "process : " << getpid() << " quit" << endl;
            exit(0);
        }
        // father
        close(pipefd[0]);      // 关r端
        // 写
        string name = "process-" + to_string(i);
        channels->push_back(channel(pipefd[1], id, name));
    }
}

// 输入型参数
void Debug(const vector<channel> &channels)
{
    for(auto&c:channels)
    {
        cout<<c._cmdfd<<" "<<c._slaverid<<" "<<c._processname<<endl;
    }
}

void Menu()
{
    std::cout << "################################################" << std::endl;
    std::cout << "# 1. 刷新日志             2. 刷新出来野怪        #" << std::endl;
    std::cout << "# 3. 检测软件是否更新      4. 更新用的血量和蓝量  #" << std::endl;
    std::cout << "#                         0. 退出               #" << std::endl;
    std::cout << "#################################################" << std::endl;
}

void ctrlSlaver(const vector<channel> &channels)
{
    // 轮询方式进行选择任务进程
    int which=0;
    while(true)
    {
        // 1.选择任务
        
        // int cmdcode=rand()%tasks.size();

        int select = 0;
        Menu();
        cout << "Please Enter@ ";
        cin >> select;
        if(select <= 0 || select >= 5) break;
        int cmdcode = select - 1;

        // 2.选择进程  使用which选择
        //int processpos=rand()%channels.size();

        cout<<"father say: "<<"cmdcode: "<<cmdcode<<" already sendto "<<channels[which]._slaverid<<
        " process name: "<< channels[which]._processname <<endl;

        which++;
        which %=channels.size();

        // 3.发送任务
        write(channels[which]._cmdfd, &cmdcode, sizeof(cmdcode));

        sleep(1);
    }
}

// 清理收尾
// 父 w    子 r 3->0
void QuitProcess(const vector<channel> &channels)
{
    // 子进程
    // 先僵尸，后会被回收
    for(const auto &c : channels) close(c._cmdfd);
    sleep(10);
    for(const auto &c : channels) waitpid(c._slaverid, nullptr, 0);
    
}


int main()
{
    // 子进程为读
    // 父进程为写

    // 先描述，在组织
    // 描述为创建管道
    // 利用fork 父进程为写端 子进程为读端
    // 建立通信匿名管道使用函数pipe

    LoadTask(&tasks);

    srand(time(nullptr)^getpid()^1023); // 种一个随机数种子

    // 在组织
    vector<channel> channels;
    // 1.初始化
    InitProcessPool(&channels);
    // Debug测试
    Debug(channels);

    // 2.开始控制子进程
    ctrlSlaver(channels);
    // 随机选择进程
    // for(int i=1;i<=100;i++)
    // {
    //     // 1.选择任务
    //     int cmdcode=rand()%tasks.size();
    //     // 2.选择进程
    //     int processpos=rand()%channels.size();

    //     cout<<"father say: "<<"cmdcode: "<<cmdcode<<" already sendto "<<channels[processpos]._slaverid<<
    //     " process name: "<< channels[processpos]._processname <<endl;

    //     // 3.发送任务
    //     write(channels[processpos]._cmdfd, &cmdcode, sizeof(cmdcode));

    //     sleep(1);
    // }
    


    // 3. 清理收尾
    // 读端正常，写端关闭，读端就会读到0，表明文件读到了结尾，不会被阻塞
    QuitProcess(channels);

    return 0;
}