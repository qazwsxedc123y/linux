#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <pthread.h>

// 对线程进行封装
// 封装后就跟c++11使用起来就一样了

static int num = 1;
typedef void (*callback_t)();

class Thread
{
public:
    static void *Routine(void *args)
    {
        Thread* thread = static_cast<Thread*>(args);
        thread->Entery();
        return nullptr;
    }
public:
    Thread(callback_t cb) : tid_(0), name_(""), start_timestamp_(0), isrunning_(false), cb_(cb)
    {}
    void Run()
    {
        name_ = "thread-" + std::to_string(num++);
        start_timestamp_ = time(nullptr);
        isrunning_ = true;
        pthread_create(&tid_, nullptr, Routine, this);
    }
    std::string Name()
    {
        return name_;
    }
    uint64_t StartTimestamp()
    {
        return start_timestamp_;
    }
    bool IsRunning()
    {
        return isrunning_;
    }
    void Join()
    {
        pthread_join(tid_, nullptr);
        isrunning_ = false;
    }
    void Entery()
    {
        cb_();
    }
    ~Thread()
    {}
private:
    pthread_t tid_;
    std::string name_;
    uint64_t start_timestamp_; // unsigned long int
    bool isrunning_;

    callback_t cb_;
};