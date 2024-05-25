#ifndef HOSHIMNET_NET_EVENTLOOPTHREADPOOL_H_
#define HOSHIMNET_NET_EVENTLOOPTHREADPOOL_H_

#include <functional>
#include <thread>
#include <vector>

namespace hoshiMNet
{
namespace net
{

class EventLoop;

class EventLoopThreadPool
{
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;

public:
    explicit EventLoopThreadPool(EventLoop* baseLoop);
    ~EventLoopThreadPool() = default;

    void start(const ThreadInitCallback& cb);
    EventLoop* getLoop();

    void setThreadNum(int numThreads) { numThreads_ = numThreads; }

private:
    EventLoop* baseLoop_;
    int numThreads_;
    size_t next_;
    std::vector<std::thread> threads_;
    std::vector<std::unique_ptr<EventLoop>> loops_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_EVENTLOOPTHREADPOOL_H_