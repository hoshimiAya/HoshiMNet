#include "ThreadPool.h"

#include <iostream>

using namespace hoshiMNet;
using namespace hoshiMNet::base;

ThreadPool::ThreadPool(size_t threadNumber, size_t queueMaxSize)
    : threadNumber_(threadNumber)
    , queue_(queueMaxSize)
    , stop_(true) {}

ThreadPool::~ThreadPool() 
{
    if (!stop_)
    {
        stop();
    }
}

void ThreadPool::start()
{
    stop_.exchange(false);
    for (size_t i = 0; i < threadNumber_; i++)
    {
        threads_.emplace_back([this]()
        {
            while (!stop_.load())
            {
                                Task task;
                queue_.pop(task);
                if (task)
                {
                    task();
                }
            }
                    });
    }
}

void ThreadPool::stop()
{
    stop_.exchange(true);
    queue_.stop();
    for (auto& thread : threads_)
    {
        thread.join();
    }
}

size_t ThreadPool::post(Task task)
{
    if (stop_)
    {
        return -1;
    }
    queue_.push(std::move(task));
    return 0;
}

void ThreadPool::waitDone()
{
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]()
    {
        return queue_.empty();
    });
    std::cout << "all tasks done" << std::endl;
    stop();
}