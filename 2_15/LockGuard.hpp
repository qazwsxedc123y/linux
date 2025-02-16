#pragma once

#include <pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t *lock) :_lock(lock)
    {}
    void Lock()
    {
        pthread_mutex_lock(_lock);
    }
    void Unlock()
    {
        pthread_mutex_unlock(_lock);
    }
    ~Mutex()
    {}
private:
    pthread_mutex_t *_lock;
};

class LockGuard
{
    public:
    LockGuard(pthread_mutex_t *lock):mutex_(lock)
    {
        mutex_.Lock();
    }
    ~LockGuard()
    {
        mutex_.Unlock();
    }
private:
    Mutex mutex_;
};