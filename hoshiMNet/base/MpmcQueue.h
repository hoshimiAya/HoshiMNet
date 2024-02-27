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
    explicit MpmcQueue(size_t maxSize);
    ~MpmcQueue();

    void push(T&& value);
    void pop(T& value);
    size_t size() const;
    size_t maxSize() const;

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