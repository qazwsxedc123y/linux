#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>

// 设置一个锁 两个条件变量

template <class T>
class BlockQueue
{
    static const int defalutnum = 20;
public:
    BlockQueue(int maxcap = defalutnum):maxcap_(maxcap)
    {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&c_cond_, nullptr);
        pthread_cond_init(&p_cond_, nullptr);
        // low_water_ = maxcap_/3;
        // high_water_ = (maxcap_*2)/3;
    }
    bool isFull()
    {
        return q_.size() == maxcap_;
    }
    bool isEmpty()
    {
        return q_.size() == 0;
    }
    void push(const T &in)
    {
        pthread_mutex_lock(&mutex_);
        // 插入的前提是：队列不满
        while(isFull()) // 如果满则等待
        {
            pthread_cond_wait(&p_cond_, &mutex_); //1. 调用的时候，自动释放锁 2.？
        }
        // 不满，插入
        q_.push(in); // 生产完后，要记得唤醒在Empty条件变量下等待的消费者线程
        // if(q_.size() > high_water_) pthread_cond_signal(&c_cond_);
        pthread_cond_signal(&c_cond_);
        pthread_mutex_unlock(&mutex_);

    }
    T pop()
    {
        pthread_mutex_lock(&mutex_);
        // 删除的前提是：队列不为空
        while(isEmpty())
        {
            pthread_cond_wait(&c_cond_, &mutex_);
        }
        T out = q_.front();
        q_.pop();
        // if(q_.size()<low_water_) pthread_cond_signal(&p_cond_)
        pthread_cond_signal(&p_cond_);
        pthread_mutex_unlock(&mutex_);

        return out;
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&c_cond_);
        pthread_cond_destroy(&p_cond_);
    }
private:
    std::queue<T> q_; // 共享资源, q被当做整体使用的，q只有一份，加锁。但是共享资源也可以被看做多份！

    int maxcap_;      // 极值
    pthread_mutex_t mutex_;
    pthread_cond_t c_cond_; // 消费
    pthread_cond_t p_cond_; // 生产


    int low_water_;
    int high_water_;
};