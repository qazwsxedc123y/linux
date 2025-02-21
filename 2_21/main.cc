#include <iostream>
#include <ctime>
#include "ThreadPool.hpp"

int main()
{
    std::cout << "process runn..." << std::endl;
    sleep(3);
    ThreadPool<int> *tp = new ThreadPool<int>();
    tp->Start();
    srand(time(nullptr) ^ getpid());

    while(true)
    {
        //1. 构建任务
        int x = rand() % 10 + 1;

        tp->Push(x);
        //2. 交给线程池处理
        std::cout << "main thread make task: " << std::endl;

        sleep(1);
    }
}