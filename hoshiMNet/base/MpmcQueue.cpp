#include "MpmcQueue.h"

using namespace hoshiMNet;
using namespace base;

template <typename T>
MpmcQueue<T>::MpmcQueue(size_t maxSize)
    : queue_(maxSize) {}

template <typename T>
MpmcQueue<T>::~MpmcQueue() {}

template <typename T>
void MpmcQueue<T>::push(T&& value)
{
    std::unique_lock<std::mutex> lock(mutex_);
    popCv_.wait(lock, [this]
    {
        return queue_.size() < maxSize_;
    });
    queue_.push_back(std::move(value));
    pushCv_.notify_one();
}

template <typename T>
void MpmcQueue<T>::pop(T& value)
{
    std::unique_lock<std::mutex> lock(mutex_);
    pushCv_.wait(lock, [this]
    {
        return !queue_.empty();
    });
    value = std::move(queue_.front());
    queue_.pop_front();
    popCv_.notify_one();
}

template <typename T>
size_t MpmcQueue<T>::size() const
{
    return queue_.size();
}

template <typename T>
size_t MpmcQueue<T>::maxSize() const
{
    return maxSize_;
}