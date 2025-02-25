#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <pthread.h>
#include <unistd.h>

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
        for (const auto &ti : threads_)
        {
            if (ti.tid_ == tid)
            {
                return ti.name_;
            }
        }
        return "None";
    }

public:
    static void *HandlerTask(void *args) // 带 static 是因为类内的函数第一个参数是默认为 this 指针
    {
        ThreadPool<T> *tp = static_cast<ThreadPool *>(args);
        // 分配任务，如果任务队列为空，则线程等待
        std::string name = tp->GetThreadName(pthread_self());
        while (true)
        {
            tp->Lock();

            while (tp->IsQueueEmpty())
            {
                tp->ThreadSleep();
            }

            // 有任务了
            T t = tp->Pop();
            // std::cout << "处理了一个任务 : " << t << "," << name << "run" << std::endl;
            tp->Unlock();
            sleep(1);

            t();
        }
    }
    void Start()
    {
        int num = threads_.size();
        for (int i = 0; i < num; i++)
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
    static ThreadPool<T> *GetInstance()
    {
        if (nullptr == tp_) // ???
        {
            pthread_mutex_lock(&lock_);
            if (nullptr == tp_)
            {
                std::cout << "log: singleton create done first!" << std::endl;
                tp_ = new ThreadPool<T>();
            }
            pthread_mutex_unlock(&lock_);
        }

        return tp_;
    }
private:
// 构建单例
    ThreadPool(int num = defalutnum) : threads_(num)
    {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&cond_, nullptr);
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }
    // 防止形成第二个对象
    ThreadPool(const ThreadPool<T>&) = delete;
    const ThreadPool<T>& operator=(const ThreadPool<T>&) = delete;
private:
    std::vector<ThreadInfo> threads_;
    std::queue<T> tasks_;

    pthread_mutex_t mutex_;
    pthread_cond_t cond_;

    static ThreadPool<T> *tp_;
    static pthread_mutex_t lock_;
};

template <class T>
ThreadPool<T> *ThreadPool<T>::tp_ = nullptr;

template <class T>
pthread_mutex_t ThreadPool<T>::lock_ = PTHREAD_MUTEX_INITIALIZER;