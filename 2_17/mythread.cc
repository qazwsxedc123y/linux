#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

#define NUM 5
int cnt = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *Count(void *args)
{
    pthread_detach(pthread_self());
    uint64_t number = (uint64_t)args;// 是一个 64 位的无符号整数类型
    cout << "pthread: " << number << " create success" << endl;

    while(1)
    {
        pthread_mutex_lock(&mutex);
        // 我们怎么知道我们要让一个线程去休眠了那？一定是临界资源不就绪，没错，临界资源也是有状态的！！
        // 你怎么知道临界资源是就绪还是不就绪的？你判断出来的！判断是访问临界资源吗？必须是的，也就是判断必须在加锁之后！！！
        pthread_cond_wait(&cond, &mutex);           //？ 为什么在这里？ 1. pthread_cond_wait让线程等待的时候，会自动释放锁！
        // 不管临界资源的状态情况
        cout << "pthread: " << number << " , cnt: " << cnt++ << endl;
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{

    for(uint64_t  i = 1; i <= NUM; i++)
    {
        pthread_t tid;
        pthread_create(&tid, nullptr, Count, (void*)i);
        usleep(1000);
    }

    sleep(3);
    std::cout << "main thread ctrl begin: " << std::endl;

    while(1)
    {
        sleep(1);
        pthread_cond_signal(&cond); //唤醒在cond的等待队列中等待的一个线程，默认都是第一个
        std::cout << "signal one thread..." << std::endl;
    }

    // 销毁
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}