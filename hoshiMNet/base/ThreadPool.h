#ifndef HOSHIMNET_BASE_THREADPOOL_H_
#define HOSHIMNET_BASE_THREADPOOL_H_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include "MpmcQueue.h"

namespace hoshiMNet
{
namespace base
{

class ThreadPool
{
public:
    using Task = std::function<void()>;

public:
    explicit ThreadPool(size_t threadNumber, size_t queueMaxSize);
    ~ThreadPool();

    void start();
    void stop();

    size_t post(Task task);
    void waitDone();

private:
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(const ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&&) = delete;

private:
    size_t threadNumber_;
    std::vector<std::thread> threads_;
    MpmcQueue<Task> queue_; // block
    std::atomic<bool> stop_;

    std::mutex mutex_;
    std::condition_variable cv_;
};

} // namespace base
} // namespace hoshiMNet

#endif // HOSHIMNET_BASE_THREADPOOL_H_