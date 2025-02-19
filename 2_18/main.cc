#include <unistd.h>
#include <ctime>
#include "BlockQueue.hpp"
#include "Task.hpp"

// 生产消费模型
// 3个生产者  5个消费者

// 加上Task 版

void *Consumer(void *args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    while (true)
    {
        // 消费
        Task t = bq->pop();
        // 计算
        // t.run();
        t();
        
        std::cout << "处理任务: " << t.GetTask() << " 运算结果是： " << t.GetResult() << " thread id: " << pthread_self() << std::endl;

        sleep(1);
    }
}

void *Productor(void *args)
{
    int len = opers.size();
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    int x = 10;
    int y = 20;
    while (true)
    {
        // 模拟生产者生产数据
        int data1 = rand() % 10 + 1; // [1,10]
        usleep(10);
        int data2 = rand() % 10;
        char op = opers[rand() % len];
        Task t(data1, data2, op);

        // 生产
        bq->push(t);
        std::cout << "生产了一个任务: " << t.GetTask() << " thread id: " << pthread_self() << std::endl;
        sleep(1);
    }
}

int main()
{
    srand(time(nullptr));

    // 因为 321 原则
    // BlockQueue 内部可不可以传递其他数据，比如对象？比如任务？？？
    BlockQueue<int> *bq = new BlockQueue<int>(5);

    pthread_t c[3], p[5];
    for (int i = 0; i < 3; i++)
    {
        pthread_create(c + i, nullptr, Consumer, bq);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_create(p + i, nullptr, Productor, bq);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(c[i], nullptr);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(p[i], nullptr);
    }
    return 0;
}

// 普通版

// void *Consumer(void *args)
// {
//     BlockQueue<int> *bq = static_cast<BlockQueue<int> *>(args);
//     while(true)
//     {
//         int tmp = rand() % 100;
//         bq->push(tmp);
//         std::cout << "生产了一个任务" << "，任务为 : " << tmp << std::endl;
//         // std::cout << "处理任务: " <<  t.GetTask()  << " 运算结果是： " << t.GetResult() << " thread id: " << pthread_self() << std::endl;

//         sleep(1);
//     }
// }

// void *Productor(void *args)
// {

//     BlockQueue<int> *bq = static_cast<BlockQueue<int> *>(args);
//     while(true)
//     {
//         int tmp = bq->pop();
//         std::cout << "消费了一个任务" << "，任务为 : " << tmp << std::endl;
//         // std::cout << "生产了一个任务: " << t.GetTask() << " thread id: " << pthread_self() << std::endl;

//     }
// }

// int main()
// {
//     srand(time(nullptr));

//     // 因为 321 原则
//     // BlockQueue 内部可不可以传递其他数据，比如对象？比如任务？？？
//     BlockQueue<int> *bq = new BlockQueue<int>(5);

//     pthread_t c[3], p[5];
//     for(int i = 0;i < 3; i++)
//     {
//         pthread_create(c + i, nullptr, Consumer, bq);
//     }

//     for (int i = 0; i < 5; i++)
//     {
//         pthread_create(p + i, nullptr, Productor, bq);
//     }

//     for (int i = 0; i < 3; i++)
//     {
//         pthread_join(c[i], nullptr);
//     }

//     for (int i = 0; i < 5; i++)
//     {
//         pthread_join(p[i], nullptr);
//     }
//     return 0;
// }