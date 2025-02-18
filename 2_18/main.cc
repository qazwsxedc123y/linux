#include <unistd.h>
#include <ctime>
#include <iostream>
#include "BlockQueue.hpp"

using namespace std;

// 生产消费模型  
// 3个生产者  5个消费者

void *Consumer(void *args)
{

}

void *Productor(void *args)
{

}

int main()
{
    pthread_t c[3], p[5];
    for(int i = 0;i < 3; i++)
    {
        pthread_create(c + i, nullptr, Consumer, nullptr);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_create(p + i, nullptr, Productor, nullptr);
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