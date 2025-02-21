#include <iostream>
#include <ctime>
#include "ThreadPool.hpp"


int main()
{
    // 如果获取单例对象的时候，也是多线程获取的呢？ --- 加锁
    std::cout << "process runn..." << std::endl;
    sleep(3);
    ThreadPool<int>::GetInstance()->Start();
    srand(time(nullptr) ^ getpid());

    while(true)
    {
        //1. 构建任务
        int x = rand() % 10 + 1;

        ThreadPool<int>::GetInstance()->Push(x);
        //2. 交给线程池处理
        std::cout << "main thread make task: " << x << std::endl;

        sleep(1);
    }
}