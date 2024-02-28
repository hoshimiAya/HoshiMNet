#ifndef HOSHIMNET_BASE_MPMCQUEUE_H_
#define HOSHIMNET_BASE_MPMCQUEUE_H_

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>

namespace hoshiMNet
{
namespace base
{

template <typename T>
class MpmcQueue
{
public:
    explicit MpmcQueue(size_t maxSize)
        : maxSize_(maxSize) {}

    ~MpmcQueue() {}

    void push(T&& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        pushCv_.wait(lock, [this]
        {
            return queue_.size() < maxSize_;
        });
        queue_.push_back(std::move(value));
        popCv_.notify_one();
    }

    void pop(T& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        popCv_.wait(lock, [this]
        {
            return !queue_.empty();
        });
        value = std::move(queue_.front());
        queue_.pop_front();
        pushCv_.notify_one();
    }

    size_t size() const { return queue_.size(); }
    size_t maxSize() const { return maxSize_; }
    bool empty() const { return queue_.empty(); }

private:
    std::mutex mutex_;
    std::condition_variable pushCv_;
    std::condition_variable popCv_;
    std::deque<T> queue_;
    std::atomic<size_t> maxSize_;
};

} // namespace base
} // namespace hoshiMNet

#endif // HOSHIMNET_BASE_MPMCQUEUE_H_