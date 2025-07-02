#include <iostream>
#include <unistd.h>
#include <queue>
#include <stdlib.h>
#include <pthread.h>

class ThreadInfo
{
public:
    pthread_t tid_;
    std::string name_;
};

static const int defalutnum = 5;

template <class T>
class ThreadPool
{
public:
    void Lock()
    {
        pthread_mutex_lock(&mutex_);
    }
    void Unlock()
    {
        pthread_mutex_unlock(&mutex_);
    }
    void Wakeup()
    {
        pthread_cond_signal(&cond_);
    }
    void ThreadSleep()
    {
        pthread_cond_wait(&cond_, &mutex_);
    }
    bool IsQueueEmpty()
    {
        return tasks_.empty();
    }
    std::string GetThreadName(pthread_t tid)
    {
        for(const auto& ti:threads_)
        {
            if(ti.tid_ == tid)
            {
                return ti.name_;
            }
        }
        return "None";
    }
public:
    static void *HandlerTask(void *args) // 带 static 是因为类内的函数第一个参数是默认为 this 指针
    {
        ThreadPool<T> *tp = static_cast<ThreadPool*>(args);
        std::string name = tp->GetThreadName(pthread_self());

        while(true)
        {
            tp->Lock();

            while(tp->IsQueueEmpty())
            {
                tp->ThreadSleep();
            }
            // 分配到任务
            T t = tp->Pop();
            std::cout<< "分配到了一个任务 : " << t << "," << name << "run" << std::endl;
            tp->Unlock();

            sleep(1);
            // 后续的任务处理部分
            // run();
        }
    }
    void Start()
    {
        int num = defalutnum;
        for(int i=0; i < num; i++)
        {
            threads_[i].name_ = "thread-" + std::to_string(i + 1);
            pthread_create(&(threads_[i].tid_), nullptr, HandlerTask, this);
        }
    }
    T Pop()
    {
        T t = tasks_.front();
        tasks_.pop();
        return t;
    }
    void Push(const T &t)
    {
        Lock();
        tasks_.push(t);
        Wakeup();
        Unlock();
    }
public:
    ThreadPool(int num = defalutnum) :threads_(num)
    {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&cond_, nullptr);
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }
private:
    std::vector<ThreadInfo> threads_;
    std::queue<T> tasks_;
    
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
};