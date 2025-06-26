#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include "RingQueue.hpp"

using namespace std;

// 单生产单消费  环形队列
// cp 问题

void *Consumer(void *args)
{
    RingQueue<int> *td = static_cast<RingQueue<int>*>(args);
    int data;
    while(true)
    {
        td->Pop(&data);
        std::cout << "Consume data done : " << data << std::endl;
    }
}

void *Productor(void *args)
{
    RingQueue<int> *td = static_cast<RingQueue<int>*>(args);
    while(true)
    {
        int data = rand() % 100 + 1;
        td->Push(data);
        std::cout << "Productor data done : " << data << std::endl;
        sleep(1);
    }
}

int main()
{
    srand(time(nullptr) ^ getpid());
    RingQueue<int> rq;
    pthread_t c,p;
    pthread_create(&p, nullptr, Productor, (void*)&rq);
    pthread_create(&c, nullptr, Consumer, (void*)&rq);


    pthread_join(c, nullptr);
    pthread_join(p, nullptr);
    return 0;
}